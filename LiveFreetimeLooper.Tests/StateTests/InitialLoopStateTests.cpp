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

    SECTION(
        "Start Message with the same command as the starting message "
        "is dispatched, sent to looptracker and changes state to RunningState")
    {
        state->handle(state, std::make_unique<StartMessage>(startingCommand));

        REQUIRE(dispatcherMock.getCommands().size() == 1);
        REQUIRE(dispatcherMock.getCommands().back().content == startingCommand);

        REQUIRE(loopTrackerMock.getCommandsStarted().size() == 1);
        REQUIRE(loopTrackerMock.getCommandsStarted().back().content == startingCommand);

        REQUIRE(dynamic_cast<RunningState*>(state.get()));
    }

    SECTION(
        "Start Message with the different command as the starting message "
        "is not dipatched, not sent to looptracker and the state does not change")
    {
        state->handle(state, std::make_unique<StartMessage>(otherCommand));

        REQUIRE(dispatcherMock.getCommands().size() == 0);

        REQUIRE(loopTrackerMock.getCommandsStarted().size() == 0);

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
        "is dispatched, the start command is sent to looptracker and changes state to RunningState "
        "ie its treated the same as a start command")
    {
        throw std::runtime_error("not implemented");
    }

    SECTION(
        "Stop message that doesn't match the starting message "
        "is immediately dispatched but ignored: does not clear the looptracker, "
        "is not sent to the loop tracker and does not change the state")
    {
        throw std::runtime_error("not implemented");
    }
}