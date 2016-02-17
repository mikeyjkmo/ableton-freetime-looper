#pragma once

#include <unordered_map>
#include "Command.hpp"

namespace LiveFreetimeLooper
{
    enum MessageType { INVALID, START, STOP };

    class CommandMappings final
    {
    private:
        std::unordered_map<Command, Command> _startsToStops;
        std::unordered_map<Command, Command> _stopsToStarts;

    public:
        CommandMappings();
        CommandMappings(const std::unordered_map<Command, Command>& startsToStops);

        void addMapping(const Command& startCommand, const Command& stopCommand);
        MessageType getMessageType(const Command& command);
        const Command& getStartCommand(const Command& stopCommand);
    };
}
