#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/ILoopTracker.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/Command.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/RunningLoop.hpp"


class MockLoopTracker final : public LiveFreetimeLooper::ILoopTracker
{
private:
    std::vector<LiveFreetimeLooper::Command> _commandsReceived;
    std::int32_t _interval;
    std::int32_t _nextRestartMessagesCallCount;
    bool _cleared;
public:
    MockLoopTracker();

    void commandReceived(LiveFreetimeLooper::Command command);
    std::vector<LiveFreetimeLooper::Command> getNextRestartCommands();
    void incrementInterval();
    void clear();

    std::vector<LiveFreetimeLooper::Command> const& getCommandsReceived() const;
    std::int32_t const& getInterval() const;
    std::int32_t const& getNextRestartMessagesCallCount() const;
    bool isCleared() const;
};