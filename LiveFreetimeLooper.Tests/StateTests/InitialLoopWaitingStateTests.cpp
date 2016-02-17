#include "../Catch/catch.hpp"

#include <cstdint>
#include <memory>

#include "../Mocks/MockEventLogger.hpp"
#include "../Mocks/MockMessageDispatcher.hpp"
#include "../Mocks/MockLoopTracker.hpp"
#include "../Mocks/MockAsyncTimerFactory.hpp"

#include "../../LiveFreetimeLooper.Core/Messaging/StartMessage.hpp"
#include "../../LiveFreetimeLooper.Core/States/StateBase.hpp"
#include "../../LiveFreetimeLooper.Core/States/CreatedState.hpp"
#include "../../LiveFreetimeLooper.Core/States/InitialLoopWaitingState.hpp"
#include "../../LiveFreetimeLooper.Core/States/InitialLoopState.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("InitialLoopWaitingState")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;
    MockLoopTracker loopTrackerMock;

    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTrackerMock, loggerMock, asyncTimerFactory);
    std::unique_ptr<StateBase> state = std::make_unique<InitialLoopWaitingState>(resources);
    std::vector<unsigned char> command = { 0, 1 };

    SECTION("InitialLoopWaitingState relays the message to the dispatcher")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        REQUIRE(dispatcherMock.getCommands().size() == 1);
        REQUIRE(dispatcherMock.getCommands().back().content == command);
    }

    SECTION("InitialLoopWaitingState relays the message to the looptracker")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        REQUIRE(loopTrackerMock.getCommandsReceived().back().content == command);
    }

    SECTION("IntialLoopWaitingState returns InitialLoopState when message received")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
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
