#include "../Catch/catch.hpp"

#include <cstdint>
#include <memory>

#include "../Mocks/MockEventLogger.hpp"
#include "../Mocks/MockMessageDispatcher.hpp"
#include "../Mocks/MockAsyncTimerFactory.hpp"

#include "../../LiveFreetimeLooper.Core/Messaging/StartMessage.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/LoopTracker.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/CommandMappings.hpp"
#include "../../LiveFreetimeLooper.Core/States/StateBase.hpp"
#include "../../LiveFreetimeLooper.Core/States/CreatedState.hpp"
#include "../../LiveFreetimeLooper.Core/States/InitialLoopWaitingState.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("Created State")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;

    LoopTracker loopTracker;
    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
    
    std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);

    SECTION("Created State acts as a transparent relay for Start Messages before StdIn supplied")
    {
        for (unsigned char i = 1; i < 21; i++)
        {
            std::vector<unsigned char> command = { 0, i };
            state->handle(state, std::make_unique<StartMessage>(command));
            REQUIRE(dispatcherMock.getCommands().size() == i);
            REQUIRE(dispatcherMock.getCommands().back().content == command);
            REQUIRE(dynamic_cast<CreatedState*>(state.get()));
        }
    }

    SECTION("Created State returns InitialLoopWaitingState when StdIn supplied")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(dynamic_cast<InitialLoopWaitingState*>(state.get()));
    }

    SECTION("Created State ats as a transparent relay for Stop Messages before StdIn supplied")
    {
        for (unsigned char i = 1; i < 21; i++)
        {
            std::vector<unsigned char> command = { 0, i };
            std::vector<unsigned char> startCommand{ 1, i };
            state->handle(state, std::make_unique<StopMessage>(command, startCommand));
            REQUIRE(dispatcherMock.getCommands().size() == i);
            REQUIRE(dispatcherMock.getCommands().back().content == command);
            REQUIRE(dynamic_cast<CreatedState*>(state.get()));
        }
    }
}
