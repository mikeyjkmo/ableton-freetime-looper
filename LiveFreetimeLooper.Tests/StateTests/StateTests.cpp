#include "../Catch/catch.hpp"

#include <memory>
#include <chrono>
#include <thread>

#include "../Mocks/MockEventLogger.hpp"
#include "../Mocks/MockMessageDispatcher.hpp"
#include "../Mocks/MockAsyncTimerFactory.hpp"
#include "../Mocks/MockAsyncTimer.hpp"

#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/StartMessage.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/LoopTracker.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/CommandMappings.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/StateBase.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/CreatedState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/InitialLoopWaitingState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/InitialLoopState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/RunningState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Utilities/AsyncTimerFactory.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("A loop is quantised, and continue to restart whilst other loops are added")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;
    CommandMappings commandMappings;
    LoopTracker loopTracker(commandMappings);
    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
    std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);
    auto send = [&state](std::vector<unsigned char> payload) { state->handle(state, std::make_unique<StartMessage>(payload)); };
    
    std::vector<unsigned char> quantisableMessagePayload = { 0, 1 };
    std::vector<unsigned char> unrelatedMessagePayload = { 12, 3 };

    // Send StdIn to move to InitialLoopWaitingState
    state->handleStdin(state, std::string("this is a string"));
    REQUIRE(dynamic_cast<InitialLoopWaitingState*>(state.get()));

    // Send in the first message, to move to InitialLoopState
    send(quantisableMessagePayload);

    REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    REQUIRE(dispatcherMock.getMessages().size() == 1);
    REQUIRE(dispatcherMock.getMessages().back().payload == quantisableMessagePayload);

    send(unrelatedMessagePayload);

    REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    REQUIRE(dispatcherMock.getMessages().size() == 2);
    REQUIRE(dispatcherMock.getMessages().back().payload == unrelatedMessagePayload);

    // Send in the second message, to move to RunningState
    send(quantisableMessagePayload);

    REQUIRE(dynamic_cast<RunningState*>(state.get()));
    REQUIRE(dispatcherMock.getMessages().size() == 3);
    REQUIRE(dispatcherMock.getMessages().back().payload == quantisableMessagePayload);

    auto timer = asyncTimerFactory.getCreatedTimersWeakRefs().back();

    // The quantised loop restart message is being dispached once per step 
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 4);
    REQUIRE(dispatcherMock.getMessages().back().payload == quantisableMessagePayload);
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 5);
    REQUIRE(dispatcherMock.getMessages().back().payload == quantisableMessagePayload);
    timer->step();
    REQUIRE(dispatcherMock.getMessages().size() == 6);
    REQUIRE(dispatcherMock.getMessages().back().payload == quantisableMessagePayload);

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

// Uses std::this_thread::sleep_for and measures outputs based on actual time.
// FIXME: RACE CONDITION MEANS THAT THIS TEST DOES NOT PASS ON ALL PLATFORMS
//TEST_CASE("The Loop quantising interval is correctly set")
//{
//    std::chrono::milliseconds wait(15);
//    std::chrono::milliseconds epsilon(2);
//
//    MockEventLogger loggerMock;
//    MockMessageDispatcher dispatcherMock;
//    LoopTracker loopTracker;
//    MockAsyncTimerFactory asyncTimerFactory;
//
//    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
//    std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);
//    auto send = [&state](std::vector<unsigned char> payload) { state->handle(state, std::make_unique<Message>(payload)); };
//
//    std::vector<unsigned char> quantisableMessagePayload = { 0, 1 };
//    state->handleStdin(state, std::string("this is a string"));
//
//    // First Loop
//    send(quantisableMessagePayload);
//    std::this_thread::sleep_for(wait);
//    send(quantisableMessagePayload);
//
//    auto timer = asyncTimerFactory.getCreatedTimersWeakRefs().back();
//    auto interval = timer->getInterval();
//
//    CAPTURE(interval.count());
//    REQUIRE(interval > wait - epsilon);
//    REQUIRE(interval < wait + epsilon);
//}

// FIXME: RACE CONDITION MEANS THAT THIS TEST DOES NOT PASS ON ALL PLATFORMS
// Uses std::this_thread::sleep_for and measures outputs based on actual time.
// TEST_CASE("The Loop quantising interval is correctly set and works as expected")
// {
//     const int waitMilliseconds = 20;
//     MockEventLogger loggerMock;
//     MockMessageDispatcher dispatcherMock;
//     LoopTracker loopTracker;
//     AsyncTimerFactory asyncTimerFactory;
// 
//     StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
//     std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);
//     auto send = [&state](std::vector<unsigned char> payload) { state->handle(state, std::make_unique<Message>(payload)); };
// 
//     std::vector<unsigned char> quantisableMessagePayload = { 0, 1 };
//     state->handleStdin(state, std::string("this is a string"));
// 
//     // First Loop
//     send(quantisableMessagePayload);
//     std::this_thread::sleep_for(std::chrono::milliseconds(waitMilliseconds));
//     send(quantisableMessagePayload);
// 
//     REQUIRE(dispatcherMock.getMessages().size() == 2);
//     std::this_thread::sleep_for(std::chrono::milliseconds((4 * waitMilliseconds) + 10));
// 
//     REQUIRE(dispatcherMock.getMessages().size() == 6);
// }
