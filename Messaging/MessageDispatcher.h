#pragma once

#include "IMessageDispatcher.h"
#include "RtMidi/RtMidi.h"

namespace LiveFreetimeLooper
{
    class Message;
    class IEventLogger;

    class MessageDispatcher final : public IMessageDispatcher
    {
    private:
        RtMidiOut& _midiOut;
        IEventLogger& _logger;
    public:
        MessageDispatcher(RtMidiOut& mOut, IEventLogger& logger);
        void sendMidiMessage(Message* message);
    };
}
