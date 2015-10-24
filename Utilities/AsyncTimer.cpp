#include "AsyncTimer.h"
#include <iostream>

using namespace std;


AsyncTimer::AsyncTimer(
        const chrono::milliseconds intervalMilliseconds, function<void()> f)
    : _intervalMilliseconds(intervalMilliseconds),
      _started(false),
      _stopped(false),
      _function(f)
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


int number = 0;
void printHello()
{
    cout << "Hello! " << number << endl;
    number++;
}


int main()
{
    AsyncTimer at(chrono::milliseconds(100), printHello);
    cout << "Starting AsyncTimer" << endl;
    at.start();
    cout << "Sleeping in main thread..." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Stopping AsyncTimer" << endl;
    at.stop();
    return 0;
}
