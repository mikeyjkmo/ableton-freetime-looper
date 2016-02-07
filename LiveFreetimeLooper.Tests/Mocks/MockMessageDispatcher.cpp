#include "MockMessageDispatcher.hpp"

MockMessageDispatcher::MockMessageDispatcher() :
    _messages()
{
}

void MockMessageDispatcher::sendMidiMessage(LiveFreetimeLooper::Message* message)
{
    _messages.push_back(*message);
}

std::vector<LiveFreetimeLooper::Message> const& MockMessageDispatcher::getMessages() const
{
    return _messages;
}