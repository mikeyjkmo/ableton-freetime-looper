#include "..\Catch\catch.hpp"

#include <cstdint>
#include <memory>

#include "..\Mocks\MockEventLogger.hpp"
#include "..\Mocks\MockMessageDispatcher.hpp"
#include "..\Mocks\MockLoopTracker.hpp"
#include "..\Mocks\MockAsyncTimerFactory.hpp"

#include "..\..\LiveFreetimeLooper.FreetimeLooper\Messaging\Message.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\StateBase.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\CreatedState.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\InitialLoopWaitingState.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\InitialLoopState.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("InitialLoopWaitingState")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;
    MockLoopTracker loopTrackerMock;

    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTrackerMock, loggerMock, asyncTimerFactory);
    std::unique_ptr<StateBase> state = std::make_unique<InitialLoopWaitingState>(resources);
    std::vector<unsigned char> messagePayload = { 0, 1 };

    SECTION("InitialLoopWaitingState relays the message to the dispatcher")
    {
        state->handle(state, std::make_unique<Message>(messagePayload));
        REQUIRE(dispatcherMock.getMessages().size() == 1);
        REQUIRE(dispatcherMock.getMessages().back() == messagePayload);
    }

    SECTION("InitialLoopWaitingState relays the message to the looptracker")
    {
        state->handle(state, std::make_unique<Message>(messagePayload));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        REQUIRE(*loopTrackerMock.getCommandsReceived().back() == messagePayload);
    }

    SECTION("IntialLoopWaitingState returns InitialLoopState when message received")
    {
        state->handle(state, std::make_unique<Message>(messagePayload));
        REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    }

    SECTION("InitialLoopWaitingState returns CreatedState when StdIn supplied")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(dynamic_cast<CreatedState*>(state.get()));
    }

    SECTION("InitialLoopWaitingState clears its LoopTracker when StdIn supplied")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(loopTrackerMock.isCleared());
    }
}