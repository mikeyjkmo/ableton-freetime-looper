#pragma once

#include <vector>
#include "IMessage.hpp"
#include "StartMessage.hpp"
#include "Command.hpp"

namespace LiveFreetimeLooper
{
    class StopMessage final : public Message
    {
    private:
        const Command& startCommand;

    public:
        StopMessage(Command command,
            const Command& startCommand) :
            Message(command),
            startCommand(startCommand)
        {
        }

        StopMessage(std::vector<unsigned char> command,
            const Command& startCommand) :
            Message(command),
            startCommand(startCommand)
        {
        }

        StopMessage(Command command,
            const Command& startCommand,
            double deltatime) :
            Message(command, deltatime),
            startCommand(startCommand)
        {
        }

        bool isMatchingStopMessageFor(StartMessage startMessage)
        {
            return startMessage.getCommand() == startCommand;
        }

        const Command& getStartCommand()
        {
            return startCommand;
        }
    };
}