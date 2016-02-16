#pragma once

#include <vector>
#include "Command.hpp"

namespace LiveFreetimeLooper
{
    enum MessageType { UNKNOWN, START, STOP };

    class CommandMappings final
    {
        // todo collection of all the loop messages we know and all their respective stop message

        // somecollection< startmessage, stoppMessage >

    public:
        CommandMappings();
        //MessageType getMessageType(const Command& command, Command* startCommand);
        MessageType getMessageType(const Command& command);
        const Command& getStartCommand(const Command& stopCommand);
    };
}