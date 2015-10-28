#pragma once

#include "RtMidi/RtMidi.h"

class EventLogger;

namespace LiveFreetimeLooper
{
    class Message;
    class EventLogger;

    class MessageDispatcher final
    {
    public:
        MessageDispatcher(RtMidiOut& mOut, EventLogger& logger);
        void sendMidiMessage(Message* message);
    private:
        RtMidiOut& _midiOut;
        EventLogger& _logger;
    };
}
