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
#include "../../LiveFreetimeLooper.Core/States/InitialLoopState.hpp"
#include "../../LiveFreetimeLooper.Core/States/RunningState.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("InitialLoopState")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;
    MockLoopTracker loopTrackerMock;

    std::vector<unsigned char> startingCommand = { 7, 1 };
    std::vector<unsigned char> otherCommand = { 3, 5 };

    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTrackerMock, loggerMock, asyncTimerFactory);
    StartMessage startingMessage(startingCommand);
    std::unique_ptr<StateBase> state = std::make_unique<InitialLoopState>(
        resources, startingMessage);

    SECTION("When InitialLoopState receives the a message with the same command as the starting message it relays the message to the dispatcher")
    {
        state->handle(state, std::make_unique<StartMessage>(startingCommand));
        REQUIRE(dispatcherMock.getCommands().size() == 1);
        REQUIRE(dispatcherMock.getCommands().back().content == startingCommand);
    }

    SECTION("When InitialLoopState receives the a message with the same command as the starting message it is relayed to the looptracker")
    {
        state->handle(state, std::make_unique<StartMessage>(startingCommand));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        REQUIRE(loopTrackerMock.getCommandsReceived().back().content == startingCommand);
    }

    SECTION("When InitialLoopState receives the a message with the same command as the starting message the state changes to RunningState")
    {
        state->handle(state, std::make_unique<StartMessage>(startingCommand));
        REQUIRE(dynamic_cast<RunningState*>(state.get()));
    }

    SECTION("When InitialLoopState receives the a message with a different command as the starting message it is not relayed to the dispatcher")
    {
        state->handle(state, std::make_unique<StartMessage>(otherCommand));
        REQUIRE(dispatcherMock.getCommands().size() == 0);
    }

    SECTION("When InitialLoopState receives the a message with a different command as the starting message the state remains unchanged")
    {
        state->handle(state, std::make_unique<StartMessage>(otherCommand));
        REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    }

    SECTION("When InitialLoopState receives the a message with a different command as the starting message it is not relayed to the looptracker")
    {
        state->handle(state, std::make_unique<StartMessage>(otherCommand));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 0);
    }


    SECTION("InitialLoopState returns CreatedState when StdIn supplied")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(dynamic_cast<CreatedState*>(state.get()));
    }

    SECTION("InitialLoopState clears its LoopTracker when StdIn supplied")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(loopTrackerMock.isCleared());
    }

    SECTION("InitialLoopState does ?? with Stop Messages")
    {
        REQUIRE("not implemented" == "");
    }
}
