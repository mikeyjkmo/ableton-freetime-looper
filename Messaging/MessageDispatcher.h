#pragma once

#include "RtMidi/RtMidi.h"

namespace AbletonProject
{
    class Message;

    class MessageDispatcher final
    {
    public:
        MessageDispatcher(RtMidiOut& mOut);
        void sendMidiMessage(Message* message);
    private:
        RtMidiOut& _midiOut;
    };
}
