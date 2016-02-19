#pragma once

#include <vector>
#include "Message.hpp"

namespace LiveFreetimeLooper
{
    class Command;

    class StartMessage final : public Message
    {
    public:
        StartMessage(Command command) :
            Message(command)
        {
        }

        StartMessage(std::vector<unsigned char> command) :
            Message(command)
        {
        }

        StartMessage(Command command, double deltatime) :
            Message(command, deltatime)
        {
        }
    };
}
