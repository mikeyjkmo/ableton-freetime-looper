#include "MockLoopTracker.h"

#include <exception>

MockLoopTracker::MockLoopTracker() :
    _commandsReceived(),
    _interval(0),
    _cleared(false)
{
}

void MockLoopTracker::incrementInterval()
{
    ++_interval;
}

void MockLoopTracker::commandReceived(std::unique_ptr<LiveFreetimeLooper::Message> message)
{
    _commandsReceived.push_back(std::move(message));
    _cleared = false;
}

std::vector<LiveFreetimeLooper::Message*> MockLoopTracker::getNextRestartMessages()
{
    throw std::runtime_error("Not Implemented");
}

void MockLoopTracker::clear()
{
    _cleared = true;
}

std::vector<std::unique_ptr<LiveFreetimeLooper::Message>> const& MockLoopTracker::getCommandsReceived() const
{
    return _commandsReceived;
}

std::int32_t const& MockLoopTracker::getInterval() const
{
    return _interval;
}

bool MockLoopTracker::isCleared() const
{
    return _cleared;
}