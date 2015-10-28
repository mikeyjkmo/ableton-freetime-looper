#include "RunningState.h"
#include "CreatedState.h"
#include "Logging/EventLogger.h"
#include "Logging/StateChangedEvent.h"
#include "Messaging/Message.h"
#include "Messaging/MessageDispatcher.h"

namespace LiveFreetimeLooper
{

    RunningState::RunningState(StateResources& resources, std::chrono::milliseconds timespan)
        : _asyncTimer(timespan, std::bind(&RunningState::_dequeueAndSendAll, this)),
          _queue(),
          _resources(resources)
    {
        _asyncTimer.start();
    }

    /*
     * _dequeueAndSendAll() is called at every interval as set on the AsyncTimer
     * (which will be the length of the initial loop) until the state
     * changes.
     */
    void RunningState::_dequeueAndSendAll()
    {
        std::unique_ptr<Message> message;
        while (_queue.tryDequeue(message))
        {
            _resources.messageDispatcher.sendMidiMessage(message.get());
            // inform the loopTracker that a command has been received
            _resources.loopTracker.commandReceived(std::move(message));
        }

        // Get vector of restart messages to send
        for (auto*& restartMessage : _resources.loopTracker.getNextRestartMessages())
        {
            _resources.messageDispatcher.sendMidiMessage(restartMessage);
        }

        _resources.loopTracker.incrementInterval();
    }

    void RunningState::handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message)
    {
        _queue.enqueue(std::move(message));
    }

    void RunningState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        _resources.loopTracker.clear();
        _resources.logger.log(std::make_unique<StateChangedEvent>(
            std::string("StdIn. Reset: Running -> Created"),
            std::string("RunningState")));
        state = std::make_unique<CreatedState>(_resources);
    }

}
