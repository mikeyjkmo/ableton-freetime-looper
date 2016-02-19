#pragma once

#include <vector>
#include "IMessage.hpp"
#include "Command.hpp"

namespace LiveFreetimeLooper
{
    class StartMessage final : public IMessage
    {
    private:
        Command command;
        double deltatime;

    public:
        StartMessage(Command command) :
            command(command), deltatime(0.0)
        {
        }

        StartMessage(std::vector<unsigned char> command) :
            command(command), deltatime(0.0)
        {
        }

        StartMessage(Command command, double deltatime) :
            command(command), deltatime(deltatime)
        {
        }

        const Command& getCommand() override
        {
            return command;
        }

        double getDeltatime() override
        {
            return deltatime;
        }

        bool operator==(const StartMessage& other) const
        {
            return other.command == command;
        }
    };
}
