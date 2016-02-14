#include "MockLoopTracker.hpp"

MockLoopTracker::MockLoopTracker() :
    _commandsReceived(),
    _interval(0),
    _nextRestartMessagesCallCount(0),
    _cleared(false)
{
}

void MockLoopTracker::incrementInterval()
{
    ++_interval;
}

void MockLoopTracker::commandReceived(std::unique_ptr<LiveFreetimeLooper::StartMessage> message)
{
    _commandsReceived.push_back(std::move(message));
    _cleared = false;
}

std::vector<LiveFreetimeLooper::StartMessage*> MockLoopTracker::getNextRestartMessages()
{
    ++_nextRestartMessagesCallCount;
    return std::vector<LiveFreetimeLooper::StartMessage*>();
}

void MockLoopTracker::clear()
{
    _cleared = true;
}

std::vector<std::unique_ptr<LiveFreetimeLooper::StartMessage>> const& MockLoopTracker::getCommandsReceived() const
{
    return _commandsReceived;
}

std::int32_t const& MockLoopTracker::getInterval() const
{
    return _interval;
}

std::int32_t const& MockLoopTracker::getNextRestartMessagesCallCount() const
{
    return _nextRestartMessagesCallCount;
}

bool MockLoopTracker::isCleared() const
{
    return _cleared;
}