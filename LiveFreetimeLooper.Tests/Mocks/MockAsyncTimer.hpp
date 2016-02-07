#pragma once

#include "..\..\LiveFreetimeLooper\Utilities\IAsyncTimer.hpp"

#include <functional>

class MockAsyncTimer final : public LiveFreetimeLooper::IAsyncTimer
{
private:
    std::function<void()> _function;
    bool _running;
public:
    MockAsyncTimer(std::function<void()> func);

    void start();
    void stop();

    void step();
    bool isRunning();
};