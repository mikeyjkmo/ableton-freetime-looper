#pragma once

#include "IAsyncTimerFactory.h"

#include <functional>
#include <chrono>
#include <memory>

namespace LiveFreetimeLooper
{
    class IAsyncTimer;

    class AsyncTimerFactory final : public IAsyncTimerFactory
    {
    public:
        AsyncTimerFactory();
        virtual std::unique_ptr<IAsyncTimer> createAsyncTimer(
            const std::chrono::duration<std::chrono::high_resolution_clock::rep,
            std::chrono::high_resolution_clock::period> interval,
            std::function<void()> f);
    };
}
