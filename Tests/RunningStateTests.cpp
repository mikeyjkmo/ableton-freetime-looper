#include "Tests\Catch\catch.h"

#include <memory>
#include <chrono>

#include "Mocks\MockEventLogger.h"
#include "Mocks\MockMessageDispatcher.h"
#include "Mocks\MockAsyncTimerFactory.h"

#include "Messaging\Message.h"
#include "Messaging\LoopTracker.h"
#include "States\StateBase.h"
#include "States\CreatedState.h"
#include "States\RunningState.h"

using namespace LiveFreetimeLooper;

TEST_CASE("Running State")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;

    LoopTracker loopTracker;
    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);

    std::unique_ptr<StateBase> state = std::make_unique<RunningState>(resources, std::chrono::high_resolution_clock::duration::zero());

    // todo more tests

    SECTION("Running State returns CreatedState when StdIn supplied")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(dynamic_cast<CreatedState*>(state.get()));
    }
}