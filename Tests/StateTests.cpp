#include "Tests\Catch\catch.h"

#include <memory>
#include <chrono>

#include "Mocks\MockEventLogger.h"
#include "Mocks\MockMessageDispatcher.h"
#include "Mocks\MockAsyncTimerFactory.h"
#include "Mocks\MockAsyncTimer.h"

#include "Messaging\Message.h"
#include "Messaging\LoopTracker.h"
#include "States\StateBase.h"
#include "States\CreatedState.h"
#include "States\InitialLoopWaitingState.h"
#include "States\InitialLoopState.h"
#include "States\RunningState.h"

TEST_CASE("States")
{
    SECTION("Running State continues to restart the original quantised to loop")
    {
        // todo (or something like that)
    }

    // todo more tests

}