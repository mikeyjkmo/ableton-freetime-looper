#include "CommandMappings.hpp"
#include <stdexcept>

namespace LiveFreetimeLooper
{
    CommandMappings::CommandMappings()
    {
    }

    // startCommand is the corresponding start command when MessageType is STOP.
    // Potential solution if getMessageType and getStartCommand need to be combined for efficiency
    /*MessageType CommandMappings::getMessageType(const Command& command, Command* startCommand)
    {
        if (startCommand != nullptr)
            throw std::runtime_error("command is an output parameter and must be passed in as nullptr");

        startCommand = nullptr;
        return MessageType::START;
    }*/

    MessageType CommandMappings::getMessageType(const Command& command)
    {
        return MessageType::START;
    }

    const Command& CommandMappings::getStartCommand(const Command& stopCommand)
    {
        if (false)
            throw std::runtime_error(
                "There is no corresponding start command. Call getMessageType to check that this is a STOP message"
                );

        throw std::runtime_error("not implemented");
    }
}