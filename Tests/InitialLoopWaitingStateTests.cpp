#include "Tests\Catch\catch.h"

#include <cstdint>
#include <memory>

#include "Mocks\MockEventLogger.h"
#include "Mocks\MockMessageDispatcher.h"

#include "Messaging\Message.h"
#include "Messaging\LoopTracker.h"
#include "States\StateBase.h"
#include "States\CreatedState.h"
#include "States\InitialLoopWaitingState.h"
#include "States\InitialLoopState.h"

using namespace LiveFreetimeLooper;

TEST_CASE("InitialLoopWaitingState")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;

    LoopTracker loopTracker;
    StateResources resources(dispatcherMock, loopTracker, loggerMock);
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
        // todo need a looptracker mock for this assertion
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
}