#include "LoopTracker.hpp"
#include "CommandMappings.hpp"

namespace LiveFreetimeLooper
{
    LoopTracker::LoopTracker() :
        _recording(),
        _running()
    {
    }

    void LoopTracker::incrementInterval()
    {
        for (auto& recordingLoop : _recording)
        {
            recordingLoop.second->incrementInterval();
        }
    }

    void LoopTracker::startCommand(Command command)
    {
        auto recordingLoopEntry = _recording.find(command);
        // If recording, move to running
        if (recordingLoopEntry != _recording.end())
        {
            // If recording stopped in the same interval as started (ie interval == 0), discard.
            if (recordingLoopEntry->second->getInterval() > 0)
            {
                _running.emplace(recordingLoopEntry->first, std::make_unique<RunningLoop>(recordingLoopEntry->second->moveToRunningLoop()));
            }

            _recording.erase(recordingLoopEntry);
            return;
        }

        auto runningMessageEntry = _running.find(command);
        // If unknown, move to recording
        if (runningMessageEntry == _running.end())
        {
            _recording.emplace(command, std::make_unique<RecordingLoop>(command));
        }
    }

    void LoopTracker::stopCommand(const Command& correspondingStartCommand)
    {
        auto recordingLoopEntry = _recording.find(correspondingStartCommand);
        // If recording, delete the loop
        if (recordingLoopEntry != _recording.end())
        {
            _recording.erase(recordingLoopEntry);
        }

        auto runningMessageEntry = _running.find(correspondingStartCommand);
        // If running, delete the loop
        if (runningMessageEntry != _running.end())
        {
            _running.erase(runningMessageEntry);
        }

        // If unknown, do nothing
    }

    std::vector<Command> LoopTracker::getNextRestartCommands()
    {
        std::vector<Command> restartMessages;

        for (auto& i : _running)
        {
            if (i.second->checkIfRestartRequired())
            {
                restartMessages.push_back(i.second->getCommand());
            }

            i.second->decrementNextRestartWait();
        }

        return restartMessages;
    }

    void LoopTracker::clear()
    {
        _recording.clear();
        _running.clear();
    }

}
