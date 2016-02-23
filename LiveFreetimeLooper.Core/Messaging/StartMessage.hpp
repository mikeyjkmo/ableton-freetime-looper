#pragma once

#include <vector>
#include "Message.hpp"

namespace LiveFreetimeLooper
{
    class Command;

    class StartMessage final : public Message
    {
    public:
        explicit StartMessage(Command command) :
            Message(command)
        {
        }

        explicit StartMessage(std::vector<unsigned char> command) :
            Message(command)
        {
        }

        explicit StartMessage(Command command, double deltatime) :
            Message(command, deltatime)
        {
        }
    };
}
