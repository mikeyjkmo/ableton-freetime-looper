#include "LoopTracker.h"

namespace AbletonProject
{
    void LoopTracker::incrementInterval()
    {
        for (auto& recordingLoop : _recording)
        {
            recordingLoop.second->incrementInterval();
        }
    }

    void LoopTracker::commandReceived(std::unique_ptr<Message> message)
    {
        auto recordingLoopEntry = _recording.find(*message);

        // If recording, move to running
        if (recordingLoopEntry != _recording.end())
        {
            _running.emplace(recordingLoopEntry->first, std::move(recordingLoopEntry->second));
            _recording.erase(recordingLoopEntry);
            return;
        }

        auto runningMessageEntry = _running.find(*message);
        // If unknown, move to recording
        if (runningMessageEntry == _running.end())
        {
            _recording.emplace(*message, std::make_unique<Loop>(std::move(message)));
        }
    }

    std::vector<Message*> LoopTracker::getNextRestartMessages()
    {
        std::vector<Message*> restartMessages;

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
