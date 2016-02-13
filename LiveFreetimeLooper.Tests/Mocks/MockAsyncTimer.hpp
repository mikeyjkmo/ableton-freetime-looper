#pragma once

#include "..\..\LiveFreetimeLooper.FreetimeLooper\Utilities\IAsyncTimer.hpp"

#include <functional>
#include <chrono>

class MockAsyncTimer final : public LiveFreetimeLooper::IAsyncTimer
{
private:
    std::function<void()> _function;
    bool _running;
    const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> _interval;
public:
    MockAsyncTimer(const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> interval,
        std::function<void()> func);

    void start();
    void stop();

    void step();
    bool isRunning();
    const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> getInterval();
};