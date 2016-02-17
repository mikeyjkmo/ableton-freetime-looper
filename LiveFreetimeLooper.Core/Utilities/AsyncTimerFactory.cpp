#include "AsyncTimerFactory.hpp"
#include "AsyncTimer.hpp"

namespace LiveFreetimeLooper
{
    AsyncTimerFactory::AsyncTimerFactory()
    {
    }

    std::unique_ptr<IAsyncTimer> AsyncTimerFactory::createAsyncTimer(
        const std::chrono::duration<std::chrono::high_resolution_clock::rep,
        std::chrono::high_resolution_clock::period> interval,
        std::function<void()> f)
    {
        return std::make_unique<AsyncTimer>(interval, f);
    }
}