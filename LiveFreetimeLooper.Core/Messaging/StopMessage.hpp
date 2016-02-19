#pragma once

#include <vector>
#include "Command.hpp"

namespace LiveFreetimeLooper
{
    class StopMessage final : public IMessage
    {
    private:
        Command command;
        const Command& startCommand;
        double deltatime;

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

        StopMessage(Command command,
            const Command& startCommand,
            double deltatime) :
            command(command),
            startCommand(startCommand),
            deltatime(deltatime)
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

        const Command& getStartCommand()
        {
            return startCommand;
        }

        bool operator==(const StopMessage& other) const
        {
            return other.command == command;
        }
    };
}