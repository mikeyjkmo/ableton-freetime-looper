#pragma once
#include <vector>
#include "..\..\LiveFreetimeLooper\Messaging\Message.h"
#include "..\..\LiveFreetimeLooper\Messaging\IMessageDispatcher.h"

class MockMessageDispatcher final : public LiveFreetimeLooper::IMessageDispatcher
{
private:
    std::vector<LiveFreetimeLooper::Message> _messages;
public:
    MockMessageDispatcher();
    void sendMidiMessage(LiveFreetimeLooper::Message* message);
    std::vector<LiveFreetimeLooper::Message> const& getMessages() const;
};