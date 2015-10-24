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
        MessageDispatcher(std::unique_ptr<RtMidiOut> mOut);
        virtual ~MessageDispatcher() = default;
        void sendMessage(Message& message);
    private:
        std::unique_ptr<RtMidiOut> _midiOut;
    };
}