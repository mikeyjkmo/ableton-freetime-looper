#pragma once

#include "Utilities\IAsyncTimerFactory.h"
#include "Utilities\IAsyncTimer.h"

#include <functional>
#include <chrono>
#include <memory>

class MockAsyncTimerFactory final : public LiveFreetimeLooper::IAsyncTimerFactory
{
public:
    MockAsyncTimerFactory();
    virtual std::unique_ptr<LiveFreetimeLooper::IAsyncTimer> createAsyncTimer(
        const std::chrono::duration<std::chrono::high_resolution_clock::rep,
        std::chrono::high_resolution_clock::period> interval,
        std::function<void()> f);
};
