#pragma once

namespace LiveFreetimeLooper
{
    class IMessage;
    class Command;

    class IMessageDispatcher
    {
    protected:
        IMessageDispatcher() {};
    public:
        virtual ~IMessageDispatcher() = default;
        virtual void sendNewMidiMessage(Command command) = 0;
        virtual void sendMidiMessage(IMessage* message) = 0;
    };
}
