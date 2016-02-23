#pragma once

#include "IAsyncTimerFactory.hpp"

#include <functional>
#include <chrono>
#include <memory>

namespace LiveFreetimeLooper
{
    class IAsyncTimer;

    class AsyncTimerFactory final : public IAsyncTimerFactory
    {
    public:
        explicit AsyncTimerFactory();
        virtual std::unique_ptr<IAsyncTimer> createAsyncTimer(
            const std::chrono::duration<std::chrono::high_resolution_clock::rep,
            std::chrono::high_resolution_clock::period> interval,
            std::function<void()> f) override;
    };
}
