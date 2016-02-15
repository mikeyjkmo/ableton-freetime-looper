#pragma once

#include <vector>
#include "Command.hpp"

namespace LiveFreetimeLooper
{
    class StopMessage final
    {
    public:
        StopMessage(Command command,
            const Command& startCommand) :
            command(command),
            startCommand(startCommand),
            deltatime(0.0)
        {
        }

        StopMessage(std::vector<unsigned char> command,
            const Command& startCommand) :
            command(command),
            startCommand(startCommand),
            deltatime(0.0)
        {
        }

        StopMessage(std::vector<unsigned char>* rawMessage,
            const Command& startCommand,
            double deltatime) :
            command(*rawMessage),
            startCommand(startCommand),
            deltatime(deltatime)
        {
        }

        Command command;
        const Command& startCommand;
        double deltatime;

        bool operator==(const StopMessage& other) const
        {
            return other.command == command;
        }
    };
}