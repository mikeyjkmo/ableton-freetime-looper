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
    
    SECTION("Start message is dispatched, send to the looptracker "
        "and changes the state to InitialLoopState")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        REQUIRE(dispatcherMock.getDispatchedCommands().back().content == command);

        REQUIRE(loopTrackerMock.getCommandsStarted().size() == 1);
        REQUIRE(loopTrackerMock.getCommandsStarted().back().content == command);

        REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    }

    SECTION("When Stdin is supplied it clears looptracker and returns state to CreatedState")
    {
        state->handleStdin(state, std::string("any string value"));

        REQUIRE(loopTrackerMock.isCleared());
        REQUIRE(dynamic_cast<CreatedState*>(state.get()));
    }

    SECTION("InitialLoopWaitingState State ats as a transparent relay for Stop Messages")
    {
        for (unsigned char i = 1; i < 21; i++)
        {
            std::vector<unsigned char> iCommandContent = { 0, i };
            std::vector<unsigned char> iStartCommandContent = { 1, i };
            Command iCommand(iCommandContent);
            Command iStartCommand(iStartCommandContent);
            state->handle(state, std::make_unique<StopMessage>(iCommand, iStartCommand));
            REQUIRE(dispatcherMock.getDispatchedCommands().size() == i);
            REQUIRE(dispatcherMock.getDispatchedCommands().back().content == iCommand.content);
            REQUIRE(dynamic_cast<InitialLoopWaitingState*>(state.get()));
        }
    }
}
