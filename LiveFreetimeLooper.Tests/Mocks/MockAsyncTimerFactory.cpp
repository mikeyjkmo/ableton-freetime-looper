#include "MockAsyncTimerFactory.hpp"
#include "MockAsyncTimer.hpp"

MockAsyncTimerFactory::MockAsyncTimerFactory()
{
}

std::unique_ptr<LiveFreetimeLooper::IAsyncTimer> MockAsyncTimerFactory::createAsyncTimer(
    const std::chrono::duration<std::chrono::high_resolution_clock::rep,
    std::chrono::high_resolution_clock::period> interval,
    std::function<void()> func)
{
    auto timer = std::make_unique<MockAsyncTimer>(func);
    _createdTimersWeakRefs.push_back(timer.get());
    return std::move(timer);
}

std::vector<MockAsyncTimer*> const& MockAsyncTimerFactory::getCreatedTimersWeakRefs() const
{
    return _createdTimersWeakRefs;
}