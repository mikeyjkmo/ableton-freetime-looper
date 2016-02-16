#include "CommandMappings.hpp"
#include <stdexcept>

namespace LiveFreetimeLooper
{
    CommandMappings::CommandMappings()
    {
    }

    // startCommand is the corresponding start command when MessageType is STOP.
    MessageType CommandMappings::getMessageType(const Command& command, Command* startCommand)
    {
        if (startCommand != nullptr)
            throw std::runtime_error("command is an output parameter and must be passed in as nullptr");

        startCommand = nullptr;
        return MessageType::START;
    }
}