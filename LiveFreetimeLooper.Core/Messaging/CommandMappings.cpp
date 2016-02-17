#include "CommandMappings.hpp"
#include <stdexcept>

namespace LiveFreetimeLooper
{
    CommandMappings::CommandMappings()
    {
    }

    CommandMappings::CommandMappings(const std::unordered_map<Command, Command>& startsToStops)
        : _startsToStops(startsToStops), _stopsToStarts()
    {
        for (auto& kv : startsToStops)
        {
            _stopsToStarts.emplace(kv.second, kv.first);
        }
    }

    void CommandMappings::addMapping(const Command& startCommand, const Command& stopCommand)
    {
        _startsToStops.emplace(startCommand, stopCommand);
        _stopsToStarts.emplace(stopCommand, startCommand);
    }

    MessageType CommandMappings::getMessageType(const Command& command)
    {
        if (_stopsToStarts.find(command) != _stopsToStarts.end())
            return MessageType::STOP;

        if (_startsToStops.find(command) != _startsToStops.end())
            return MessageType::START;

        return MessageType::INVALID;
    }

    const Command& CommandMappings::getStartCommand(const Command& stopCommand)
    {
        auto startCommandIter = _stopsToStarts.find(stopCommand);
        if (startCommandIter == _stopsToStarts.end())
        {
            throw std::runtime_error(
                "There is no corresponding start command. Call getMessageType to check that "
                "this is a STOP message");
        }
        return startCommandIter->second;
    }
}
