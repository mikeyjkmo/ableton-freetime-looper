#pragma once
#include <vector>
#include "Messaging\Message.h"
#include "Messaging\IMessageDispatcher.h"

class MockMessageDispatcher final : public LiveFreetimeLooper::IMessageDispatcher
{
private:
    std::vector<LiveFreetimeLooper::Message> _messages;
public:
    MockMessageDispatcher();
    void sendMidiMessage(LiveFreetimeLooper::Message* message);
    std::vector<LiveFreetimeLooper::Message> const& getMessages() const;
};