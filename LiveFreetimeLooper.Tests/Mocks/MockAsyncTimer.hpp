#pragma once

#include "../../LiveFreetimeLooper.Core/Utilities/IAsyncTimer.hpp"

#include <functional>
#include <chrono>
#include <cstdint>

class MockAsyncTimer final : public LiveFreetimeLooper::IAsyncTimer
{
private:
    std::function<void()> _function;
    bool _running;
    const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> _interval;
public:
    explicit MockAsyncTimer(const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> interval,
        std::function<void()> func);

    void start() override;
    void stop() override;

    void step();
    void step(std::int32_t times);
    bool isRunning();
    const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> getInterval();
};
