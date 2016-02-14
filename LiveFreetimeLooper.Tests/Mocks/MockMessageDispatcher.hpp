#pragma once
#include <vector>
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/StartMessage.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/IMessageDispatcher.hpp"

class MockMessageDispatcher final : public LiveFreetimeLooper::IMessageDispatcher
{
private:
    std::vector<LiveFreetimeLooper::StartMessage> _messages;
public:
    MockMessageDispatcher();
    void sendMidiMessage(LiveFreetimeLooper::StartMessage* message);
    std::vector<LiveFreetimeLooper::StartMessage> const& getMessages() const;
};
