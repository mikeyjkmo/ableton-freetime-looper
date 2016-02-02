#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "Messaging\ILoopTracker.h"
#include "Messaging\Message.h"
#include "Messaging\Loop.h"


class MockLoopTracker final : public LiveFreetimeLooper::ILoopTracker
{
private:
    std::vector<std::unique_ptr<LiveFreetimeLooper::Message>> _commandsReceived;
    std::int32_t _interval;
    bool _cleared;
public:
    MockLoopTracker();

    void commandReceived(std::unique_ptr<LiveFreetimeLooper::Message> message);
    std::vector<LiveFreetimeLooper::Message*> getNextRestartMessages();
    void incrementInterval();
    void clear();

    std::vector<std::unique_ptr<LiveFreetimeLooper::Message>> const& getCommandsReceived() const;
    std::int32_t const& getInterval() const;
    bool isCleared() const;
};