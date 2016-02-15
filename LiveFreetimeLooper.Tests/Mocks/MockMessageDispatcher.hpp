#pragma once
#include <vector>
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/Command.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/StartMessage.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/IMessageDispatcher.hpp"

class MockMessageDispatcher final : public LiveFreetimeLooper::IMessageDispatcher
{
private:
    std::vector<LiveFreetimeLooper::Command> _commands;
public:
    MockMessageDispatcher();
    void sendNewMidiMessage(LiveFreetimeLooper::Command command);
    void sendMidiMessage(LiveFreetimeLooper::StartMessage* message);
    std::vector<LiveFreetimeLooper::Command> const& getCommands() const;
};
