#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "../../LiveFreetimeLooper.Core/Messaging/ILoopTracker.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/Command.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/RunningLoop.hpp"


class MockLoopTracker final : public LiveFreetimeLooper::ILoopTracker
{
private:
    std::vector<LiveFreetimeLooper::Command> _commandsStarted;
    std::vector<LiveFreetimeLooper::Command> _commandsStopped;
    std::int32_t _interval;
    std::int32_t _nextRestartMessagesCallCount;
    bool _cleared;
public:
    explicit MockLoopTracker();

    void startCommand(LiveFreetimeLooper::Command command) override;
    void stopCommand(const LiveFreetimeLooper::Command& correspondingStartCommand) override;
    std::vector<LiveFreetimeLooper::Command> getNextRestartCommands() override;
    void incrementInterval() override;
    void clear() override;

    std::vector<LiveFreetimeLooper::Command> const& getCommandsStarted() const;
    std::vector<LiveFreetimeLooper::Command> const& getCommandsStopped() const;
    std::int32_t const& getInterval() const;
    std::int32_t const& getNextRestartMessagesCallCount() const;
    bool isCleared() const;
};
