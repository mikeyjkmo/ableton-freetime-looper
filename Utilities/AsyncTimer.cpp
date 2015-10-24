#include "AsyncTimer.h"
#include <iostream>

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


    AsyncTimer::~AsyncTimer() {}


    void AsyncTimer::start()
    {
        if (!_started)
        {
            _started = true;
            _scheduleNextCall();
        }
    }


    void AsyncTimer::stop()
    {
        _stopped = true;
        if (_previousThread)
        {
            _previousThread->join();
        }
        if (_thread)
        {
            _thread->join();
        }
    }


    void AsyncTimer::_scheduleNextCall()
    {
        if (!_stopped)
        {
            if (_previousThread)
            {
                _previousThread->join();
            }
            if (_thread)
            {
                _previousThread = _thread;
            }

            _thread = make_shared<thread>(
                [this]()
                {
                    this_thread::sleep_for(this->_intervalMilliseconds);
                    this->_function();
                    this->_scheduleNextCall();
                }
            );
        }
    }

}
