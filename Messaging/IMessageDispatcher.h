#pragma once

namespace LiveFreetimeLooper
{
    class Message;

    class IMessageDispatcher
    {
    protected:
        IMessageDispatcher() {};
    public:
        virtual ~IMessageDispatcher() = default;
        virtual void sendMidiMessage(Message* message) = 0;
    };
}
