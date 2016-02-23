#pragma once

#include <functional>
#include <chrono>
#include <memory>

namespace LiveFreetimeLooper
{
    class IAsyncTimer;

    class IAsyncTimerFactory
    {
    protected:
        explicit IAsyncTimerFactory() {};
    public:
        virtual ~IAsyncTimerFactory() = default;
        virtual std::unique_ptr<IAsyncTimer> createAsyncTimer(
            const std::chrono::duration<std::chrono::high_resolution_clock::rep,
            std::chrono::high_resolution_clock::period> interval,
            std::function<void()> f) = 0;
    };
}
