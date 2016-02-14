#include "LoopTracker.hpp"
#include "CommandMappings.hpp"

namespace LiveFreetimeLooper
{
    LoopTracker::LoopTracker(const CommandMappings& commandMappings) :
        _recording(),
        _running(),
        _commandMappings(commandMappings)
    {
    }

    void LoopTracker::incrementInterval()
    {
        for (auto& recordingLoop : _recording)
        {
            recordingLoop.second->incrementInterval();
        }
    }

    void LoopTracker::commandReceived(std::unique_ptr<StartMessage> message)
    {
        auto messageKey = *message;
        auto recordingLoopEntry = _recording.find(messageKey);

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

        auto runningMessageEntry = _running.find(messageKey);
        // If unknown, move to recording
        if (runningMessageEntry == _running.end())
        {
            _recording.emplace(messageKey, std::make_unique<RecordingLoop>(std::move(message)));
        }
    }

    std::vector<StartMessage*> LoopTracker::getNextRestartMessages()
    {
        std::vector<StartMessage*> restartMessages;

        for (auto& i : _running)
        {
            if (i.second->checkIfRestartRequired())
            {
                restartMessages.push_back(i.second->getControlMessage());
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
