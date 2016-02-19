#pragma once

namespace LiveFreetimeLooper
{
    class Command;

    class IMessage
    {
    protected:
        IMessage() {}
    public:
        virtual ~IMessage() = default;
        virtual const Command& getCommand() = 0;
        virtual double getDeltatime() = 0;
    };
}
