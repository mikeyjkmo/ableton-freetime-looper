#include "Tests\Catch\catch.h"

#include <cstdint>
#include <memory>

#include "Tests\Mocks\MockEventLogger.h"
#include "Tests\Mocks\MockMessageDispatcher.h"
#include "Tests\Mocks\MockAsyncTimerFactory.h"

#include "Messaging\Message.h"
#include "Messaging\LoopTracker.h"
#include "States\StateBase.h"
#include "States\CreatedState.h"
#include "States\InitialLoopWaitingState.h"

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