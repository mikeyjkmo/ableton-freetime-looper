#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/ILoopTracker.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/StartMessage.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/RunningLoop.hpp"


class MockLoopTracker final : public LiveFreetimeLooper::ILoopTracker
{
private:
    std::vector<std::unique_ptr<LiveFreetimeLooper::StartMessage>> _commandsReceived;
    std::int32_t _interval;
    std::int32_t _nextRestartMessagesCallCount;
    bool _cleared;
public:
    MockLoopTracker();

    void commandReceived(std::unique_ptr<LiveFreetimeLooper::StartMessage> message);
    std::vector<LiveFreetimeLooper::StartMessage*> getNextRestartMessages();
    void incrementInterval();
    void clear();

    std::vector<std::unique_ptr<LiveFreetimeLooper::StartMessage>> const& getCommandsReceived() const;
    std::int32_t const& getInterval() const;
    std::int32_t const& getNextRestartMessagesCallCount() const;
    bool isCleared() const;
};
