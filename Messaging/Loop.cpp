#include "Loop.h"

namespace AbletonProject
{
    Loop::Loop(std::unique_ptr<Message> controlMessage) :
        _controlMessage(std::move(controlMessage)),
        _interval(0)
    {

    }

    void Loop::incrementInterval()
    {
        ++_interval;
    }

    bool Loop::checkIfRestartRequired()
    {
        return _waitUntilNextRestart < 1;
    }

    void Loop::decrementNextRestartWait()
    {
        if (_waitUntilNextRestart < 1)
        {
            _waitUntilNextRestart = _interval - 1;
            return;
        }

        --_waitUntilNextRestart;
    }

    Message* Loop::getControlMessage()
    {
        return _controlMessage.get();
    }
}
