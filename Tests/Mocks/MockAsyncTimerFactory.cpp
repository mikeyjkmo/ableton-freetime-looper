#include "MockAsyncTimerFactory.h"
#include "MockAsyncTimer.h"

MockAsyncTimerFactory::MockAsyncTimerFactory()
{
}

std::unique_ptr<LiveFreetimeLooper::IAsyncTimer> MockAsyncTimerFactory::createAsyncTimer(
    const std::chrono::duration<std::chrono::high_resolution_clock::rep,
    std::chrono::high_resolution_clock::period> interval,
    std::function<void()> f)
{
    return std::make_unique<MockAsyncTimer>();
}