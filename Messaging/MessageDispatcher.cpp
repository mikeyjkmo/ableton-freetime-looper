#include "MessageDispatcher.h"

#include <iostream>

namespace AbletonProject
{

    MessageDispatcher::MessageDispatcher(RtMidiOut& mOut)
        : _midiOut(mOut)
    {
    }

    void MessageDispatcher::sendMessage(Message& message)
    {
        _midiOut.sendMessage(&(message.payload));
    }

}
