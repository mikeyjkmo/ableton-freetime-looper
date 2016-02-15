#pragma once

namespace LiveFreetimeLooper
{
    class StartMessage;
    class Command;

    class IMessageDispatcher
    {
    protected:
        IMessageDispatcher() {};
    public:
        virtual ~IMessageDispatcher() = default;
        virtual void sendNewMidiMessage(Command command) = 0;
        virtual void sendMidiMessage(StartMessage* message) = 0;
    };
}
