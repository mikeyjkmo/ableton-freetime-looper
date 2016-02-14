#include "MockMessageDispatcher.hpp"

MockMessageDispatcher::MockMessageDispatcher() :
    _messages()
{
}

void MockMessageDispatcher::sendMidiMessage(LiveFreetimeLooper::StartMessage* message)
{
    _messages.push_back(*message);
}

std::vector<LiveFreetimeLooper::StartMessage> const& MockMessageDispatcher::getMessages() const
{
    return _messages;
}