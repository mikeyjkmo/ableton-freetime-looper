#include "../Catch/catch.hpp"

#include <cstdint>
#include <memory>

#include "../Mocks/MockEventLogger.hpp"
#include "../Mocks/MockMessageDispatcher.hpp"
#include "../Mocks/MockAsyncTimerFactory.hpp"

#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/Message.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/LoopTracker.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/StateBase.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/CreatedState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/InitialLoopWaitingState.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("Created State")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;

    LoopTracker loopTracker;
    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
    
    std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);

    SECTION("Created State acts as a transparent relay before StdIn supplied")
    {
        for (unsigned char i = 1; i < 21; i++)
        {
            std::vector<unsigned char> messagePayload = { 0, i };
            state->handle(state, std::make_unique<Message>(messagePayload));
            REQUIRE(dispatcherMock.getMessages().size() == i);
            REQUIRE(dispatcherMock.getMessages().back() == messagePayload);
        }
    }

    SECTION("Created State returns InitialLoopWaitingState when StdIn supplied")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(dynamic_cast<InitialLoopWaitingState*>(state.get()));
    }
}
