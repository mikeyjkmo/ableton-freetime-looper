#include "MockMessageDispatcher.hpp"

MockMessageDispatcher::MockMessageDispatcher() :
    _commands()
{
}

void MockMessageDispatcher::sendNewMidiMessage(LiveFreetimeLooper::Command command)
{
    _commands.push_back(command);
}

void MockMessageDispatcher::sendMidiMessage(LiveFreetimeLooper::StartMessage* message)
{
    _commands.push_back(message->command);
}

std::vector<LiveFreetimeLooper::Command> const& MockMessageDispatcher::getDispatchedCommands() const
{
    return _commands;
}
