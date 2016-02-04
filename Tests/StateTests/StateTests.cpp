#include "Tests\Catch\catch.h"

#include <memory>
#include <chrono>

#include "Tests\Mocks\MockEventLogger.h"
#include "Tests\Mocks\MockMessageDispatcher.h"
#include "Tests\Mocks\MockAsyncTimerFactory.h"
#include "Tests\Mocks\MockAsyncTimer.h"

#include "Messaging\Message.h"
#include "Messaging\LoopTracker.h"
#include "States\StateBase.h"
#include "States\CreatedState.h"
#include "States\InitialLoopWaitingState.h"
#include "States\InitialLoopState.h"
#include "States\RunningState.h"

using namespace LiveFreetimeLooper;

TEST_CASE("States")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;

    LoopTracker loopTracker;
    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);

    std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);

    // Send StdIn to move to InitialLoopWaitingState
    state->handleStdin(state, std::string("this is a string"));
    REQUIRE(dynamic_cast<InitialLoopWaitingState*>(state.get()));

    //state->handle(state, std::make_unique<Message>(messagePayload));
    //REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));




    SECTION("Running State continues to restart the original loop quantised to")
    {
        // todo (or something like that)
    }

    // todo more tests

}