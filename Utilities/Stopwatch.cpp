#include "Stopwatch.h"

namespace AbletonProject
{
    using namespace std::chrono;

    Stopwatch::Stopwatch() :
        _isRunning(false),
        _elapsed(high_resolution_clock::duration::zero()),
        _startTime(high_resolution_clock::duration::zero())
    {
    }

    void Stopwatch::start()
    {
        if (_isRunning) return;

        _startTime = high_resolution_clock::now();
        _isRunning = true;
    }

    void Stopwatch::stop()
    {
        if (!_isRunning) return;
        _elapsed = calculateElapsed();
        _isRunning = false;
    }

    void Stopwatch::reset()
    {
        _elapsed = high_resolution_clock::duration::zero();
        _isRunning = false;
    }

    void Stopwatch::restart()
    {
        reset();
        start();
    }

    bool Stopwatch::IsRunning()
    {
        return _isRunning;
    }

    milliseconds Stopwatch::getElapsedMilliseconds() const
    {
        return duration_cast<milliseconds>(getElapsed());
    }

    duration<high_resolution_clock::rep, high_resolution_clock::period> Stopwatch::getElapsed() const
    {
        if (!_isRunning) return _elapsed;
        return calculateElapsed();
    }

    duration<high_resolution_clock::rep, high_resolution_clock::period> Stopwatch::calculateElapsed() const
    {
        return _elapsed + high_resolution_clock::now() - _startTime;
    }
}
