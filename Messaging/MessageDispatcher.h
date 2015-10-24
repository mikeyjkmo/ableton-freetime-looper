#pragma once

#include <string>
#include <iostream>
#include "Message.h"
#include "RtMidi/RtMidi.h"

namespace AbletonProject
{
    class MessageDispatcher
    {
    public:
        MessageDispatcher(RtMidiOut& mOut);
        virtual ~MessageDispatcher() = default;
        void sendMessage(Message& message);
    private:
        RtMidiOut& _midiOut;
    };
}