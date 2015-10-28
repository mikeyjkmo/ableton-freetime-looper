#include <iostream>
#include <cstdio>
#include "AsyncTimer.h"

using namespace std;

namespace LiveFreetimeLooper
{

    AsyncTimer::AsyncTimer(
        const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> interval,
            function<void()> f)
            : _interval(interval),
          _function(f),
          _started(false),
          _stopped(false)
    {
    }


    AsyncTimer::~AsyncTimer()
    {
        stop();
    }


    void AsyncTimer::start()
    {
        if (!_started)
        {
            _started = true;
            _scheduleNextCall();
        }
    }

    void AsyncTimer::_joinThreads(bool joinCurrent)
    {
        if (_previousThread && _previousThread->joinable())
        {
            _previousThread->join();
        }
        if (joinCurrent && _currentThread && _currentThread->joinable())
        {
            _currentThread->join();
        }
    }

    void AsyncTimer::stop()
    {
        _stopped = true;
        _joinThreads(true);
    }

    void AsyncTimer::_threadCallback()
    {
        this_thread::sleep_for(_interval);
        if (!_stopped)
        {
            _function();
            _scheduleNextCall();
        }
    }

    void AsyncTimer::_scheduleNextCall()
    {
        if (!_stopped)
        {
            _joinThreads(false);
            if (_currentThread)
            {
                _previousThread = std::move(_currentThread);
            }

            _currentThread = make_unique<thread>(
                std::bind(&AsyncTimer::_threadCallback, this));
        }
    }

}
