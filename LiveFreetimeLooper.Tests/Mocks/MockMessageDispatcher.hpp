#pragma once
#include <vector>
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/Message.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/IMessageDispatcher.hpp"

class MockMessageDispatcher final : public LiveFreetimeLooper::IMessageDispatcher
{
private:
    std::vector<LiveFreetimeLooper::Message> _messages;
public:
    MockMessageDispatcher();
    void sendMidiMessage(LiveFreetimeLooper::Message* message);
    std::vector<LiveFreetimeLooper::Message> const& getMessages() const;
};
