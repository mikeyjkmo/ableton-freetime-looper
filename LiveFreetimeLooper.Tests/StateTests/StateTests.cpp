#include "..\Catch\catch.hpp"

#include <memory>
#include <chrono>

#include "..\Mocks\MockEventLogger.hpp"
#include "..\Mocks\MockMessageDispatcher.hpp"
#include "..\Mocks\MockAsyncTimerFactory.hpp"
#include "..\Mocks\MockAsyncTimer.hpp"

#include "..\..\LiveFreetimeLooper.FreetimeLooper\Messaging\Message.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\Messaging\LoopTracker.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\StateBase.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\CreatedState.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\InitialLoopWaitingState.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\InitialLoopState.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\RunningState.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("A loop is quantised, and continue to restart whilst other loops are added")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;
    LoopTracker loopTracker;
    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
    std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);
    auto send = [&state](std::vector<unsigned char> payload) { state->handle(state, std::make_unique<Message>(payload)); };
    
    std::vector<unsigned char> quantisableMessagePayload = { 0, 1 };
    std::vector<unsigned char> unrelatedMessagePayload = { 12, 3 };

    // Send StdIn to move to InitialLoopWaitingState
    state->handleStdin(state, std::string("this is a string"));
    REQUIRE(dynamic_cast<InitialLoopWaitingState*>(state.get()));

    // Send in the first message, to move to InitialLoopState
    send(quantisableMessagePayload);

    REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    REQUIRE(dispatcherMock.getMessages().size() == 1);
    REQUIRE(dispatcherMock.getMessages().back() == quantisableMessagePayload);

    send(unrelatedMessagePayload);

    REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    REQUIRE(dispatcherMock.getMessages().size() == 2);
    REQUIRE(dispatcherMock.getMessages().back() == unrelatedMessagePayload);

    // Send in the second message, to move to RunningState
    send(quantisableMessagePayload);

    REQUIRE(dynamic_cast<RunningState*>(state.get()));
    REQUIRE(dispatcherMock.getMessages().size() == 3);
    REQUIRE(dispatcherMock.getMessages().back() == quantisableMessagePayload);

    auto timer = asyncTimerFactory.getCreatedTimersWeakRefs().back();

    // The quantised loop restart message is being dispached once per step 
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 4);
    REQUIRE(dispatcherMock.getMessages().back() == quantisableMessagePayload);
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 5);
    REQUIRE(dispatcherMock.getMessages().back() == quantisableMessagePayload);
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 6);
    REQUIRE(dispatcherMock.getMessages().back() == quantisableMessagePayload);

    // Introduce more loops
    // => A) Loop of length 1 (original quantised) + B) Loop of length 1 + C) Loop of length 2
    std::vector<unsigned char> loopOfLengthOneMessagePayload = { 2, 54 };
    std::vector<unsigned char> loopOfLengthTwoMessagePayload = { 24, 4 };

    send(loopOfLengthOneMessagePayload);
    REQUIRE(dispatcherMock.getMessages().size() == 6);
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 8);
    send(loopOfLengthTwoMessagePayload);
    send(loopOfLengthOneMessagePayload);
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 12);
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 14);
    send(loopOfLengthTwoMessagePayload);
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 18);
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 20); // +2: A and B restart 
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 23); // +3: A, B and C restart
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 25);
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 28);
}

// todo. "The timespan passed to RunningState's async timer is equal to the time waited by InitialLoopState")

// todo MessageCancellingIdeas: have message cancelling involved