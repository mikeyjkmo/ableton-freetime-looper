#include "MockAsyncTimer.hpp"

MockAsyncTimer::MockAsyncTimer(std::function<void()> func) :
    _function(func),
    _running(false)
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