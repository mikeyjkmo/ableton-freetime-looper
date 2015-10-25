#include "AsyncTimer.h"

using namespace std;

namespace AbletonProject
{

    AsyncTimer::AsyncTimer(
            const chrono::milliseconds intervalMilliseconds, function<void()> f)
        : _intervalMilliseconds(intervalMilliseconds),
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
        if (joinCurrent && _thread && _thread->joinable())
        {
            _thread->join();
        }
    }

    void AsyncTimer::stop()
    {
        _stopped = true;
        _joinThreads(true);
    }

    void AsyncTimer::_threadCallback()
    {
        this_thread::sleep_for(_intervalMilliseconds);
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
            if (_thread)
            {
                _previousThread = _thread;
            }

            _thread = make_shared<thread>(
                std::bind(&AsyncTimer::_threadCallback, this));
        }
    }

}
