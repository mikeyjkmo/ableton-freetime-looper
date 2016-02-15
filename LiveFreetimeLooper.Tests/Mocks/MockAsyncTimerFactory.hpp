#pragma once

#include "../../LiveFreetimeLooper.FreetimeLooper/Utilities/IAsyncTimerFactory.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Utilities/IAsyncTimer.hpp"

#include <functional>
#include <chrono>
#include <memory>
#include <vector>

class MockAsyncTimer;

class MockAsyncTimerFactory final : public LiveFreetimeLooper::IAsyncTimerFactory
{
private:
    std::vector<MockAsyncTimer*> _createdTimersWeakRefs;
public:
    MockAsyncTimerFactory();
    virtual std::unique_ptr<LiveFreetimeLooper::IAsyncTimer> createAsyncTimer(
        const std::chrono::duration<std::chrono::high_resolution_clock::rep,
        std::chrono::high_resolution_clock::period> interval,
        std::function<void()> func) override;

    // Here be dragons: The caller of createAsyncTimer owns the MockAsyncTimer*, and may have already deleted it
    std::vector<MockAsyncTimer*> const& getCreatedTimersWeakRefs() const;
};
