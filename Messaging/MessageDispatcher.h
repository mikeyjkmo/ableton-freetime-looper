#pragma once

#include <string>
#include <iostream>
#include "Message.h"
#include "RtMidi/RtMidi.h"

namespace AbletonProject
{
    class MessageDispatcher final
    {
    public:
        MessageDispatcher(RtMidiOut& mOut);
        void sendMidiMessage(Message& message);
    private:
        RtMidiOut& _midiOut;
    };
}
