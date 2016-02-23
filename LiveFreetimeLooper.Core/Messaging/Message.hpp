#pragma once

#include <vector>
#include "IMessage.hpp"
#include "Command.hpp"

namespace LiveFreetimeLooper
{
    class Message : public IMessage
    {
    private:
        Command command;
        double deltatime;

    public:
        explicit Message(Command command) :
            command(command), deltatime(0.0)
        {
        }

        explicit Message(std::vector<unsigned char> command) :
            command(command), deltatime(0.0)
        {
        }

        explicit Message(Command command, double deltatime) :
            command(command), deltatime(deltatime)
        {
        }

        virtual ~Message() = default;

        const Command& getCommand() override
        {
            return command;
        }

        double getDeltatime() override
        {
            return deltatime;
        }

        bool operator==(const Message& other) const
        {
            return other.command == command;
        }
    };
}
