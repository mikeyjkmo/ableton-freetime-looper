#include "MockLoopTracker.hpp"

MockLoopTracker::MockLoopTracker() :
    _commandsStarted(),
    _commandsStopped(),
    _interval(0),
    _nextRestartMessagesCallCount(0),
    _cleared(false)
{
}

void MockLoopTracker::incrementInterval()
{
    ++_interval;
}

void MockLoopTracker::startCommand(LiveFreetimeLooper::Command command)
{
    _commandsStarted.push_back(command);
    _cleared = false;
}

void MockLoopTracker::stopCommand(const LiveFreetimeLooper::Command& correspondingStartCommand)
{
    _commandsStopped.push_back(correspondingStartCommand);
    _cleared = false;
}

std::vector<LiveFreetimeLooper::Command> MockLoopTracker::getNextRestartCommands()
{
    ++_nextRestartMessagesCallCount;
    return std::vector<LiveFreetimeLooper::Command>();
}

void MockLoopTracker::clear()
{
    _cleared = true;
}

std::vector<LiveFreetimeLooper::Command> const& MockLoopTracker::getCommandsStarted() const
{
    return _commandsStarted;
}

std::vector<LiveFreetimeLooper::Command> const& MockLoopTracker::getCommandsStopped() const
{
    return _commandsStopped;
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