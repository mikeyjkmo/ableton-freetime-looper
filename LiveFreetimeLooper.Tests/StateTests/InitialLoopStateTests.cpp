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
    std::vector<unsigned char> stopCommand = { 8, 1 };
    std::vector<unsigned char> otherCommand = { 3, 5 };

    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTrackerMock, loggerMock, asyncTimerFactory);
    StartMessage startingMessage(startingCommand);
    std::unique_ptr<StateBase> state = std::make_unique<InitialLoopState>(
        resources, startingMessage);

    SECTION(
        "Start Message with the same command as the starting message "
        "is dispatched, sent to looptracker and changes state to RunningState")
    {
        state->handle(state, std::make_unique<StartMessage>(startingCommand));

        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        REQUIRE(dispatcherMock.getDispatchedCommands().back().content == startingCommand);
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        REQUIRE(loopTrackerMock.getCommandsReceived().back().content == startingCommand);

        REQUIRE(dynamic_cast<RunningState*>(state.get()));
    }

    SECTION(
        "Start Message with the different command as the starting message "
        "is not dipatched, not sent to looptracker and the state does not change")
    {
        state->handle(state, std::make_unique<StartMessage>(otherCommand));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 0);
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 0);
        REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    }


    SECTION("When Stdin is supplied it clears looptracker and returns state to CreatedState")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(loopTrackerMock.isCleared());
        REQUIRE(dynamic_cast<CreatedState*>(state.get()));
    }

    SECTION(
        "Stop message that matches the starting message "
        "is dispatched, the START command is sent to looptracker and changes state to "
        "RunningState ie its treated the same as a start command")
    {
        state->handle(state, std::make_unique<StartMessage>(startingCommand));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        REQUIRE(dispatcherMock.getDispatchedCommands().back().content == startingCommand);
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        REQUIRE(loopTrackerMock.getCommandsReceived().back().content == startingCommand);
        REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));

        state->handle(state, std::make_unique<StopMessage>(stopCommand, startingCommand));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 2);
        REQUIRE(loopTrackerMock.getCommandsReceived().back().content == startingCommand);
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 2);
        REQUIRE(dispatcherMock.getDispatchedCommands().back().content == stopCommand);
        REQUIRE(dynamic_cast<RunningState*>(state.get()));
    }

    SECTION(
        "Stop message that doesn't match the starting message is immediately "
        "dispatched but ignored: does not clear the looptracker, is not sent "
        "to the loop tracker and does not change the state")
    {
        state->handle(state, std::make_unique<StartMessage>(startingCommand));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        REQUIRE(dispatcherMock.getDispatchedCommands().back().content == startingCommand);
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        REQUIRE(loopTrackerMock.getCommandsReceived().back().content == startingCommand);
        REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));

        state->handle(state, std::make_unique<StopMessage>(stopCommand, otherCommand));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        REQUIRE(loopTrackerMock.getCommandsReceived().back().content == startingCommand);
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 2);
        REQUIRE(dispatcherMock.getDispatchedCommands().back().content == stopCommand);
        REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    }
}
