#include "MockAsyncTimer.hpp"

MockAsyncTimer::MockAsyncTimer(const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> interval,
    std::function<void()> func) :
    _function(func),
    _running(false),
    _interval(interval)
{
}

void MockAsyncTimer::start()
{
    _running = true;
}

void MockAsyncTimer::stop()
{
    _running = false;
}

void MockAsyncTimer::step()
{
    _function();
}

bool MockAsyncTimer::isRunning()
{
    return _running;
}

const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> MockAsyncTimer::getInterval()
{
    return _interval;
}