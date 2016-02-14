#pragma once

namespace LiveFreetimeLooper
{
    class StartMessage;

    class IMessageDispatcher
    {
    protected:
        IMessageDispatcher() {};
    public:
        virtual ~IMessageDispatcher() = default;
        virtual void sendMidiMessage(StartMessage* message) = 0;
    };
}
