#include "RunningState.hpp"
#include "CreatedState.hpp"
#include "../Logging/IEventLogger.hpp"
#include "../Logging/StateChangedEvent.hpp"
#include "../Messaging/StartMessage.hpp"
#include "../Messaging/StopMessage.hpp"
#include "../Utilities/IAsyncTimerFactory.hpp"

namespace LiveFreetimeLooper
{
    RunningState::RunningState(StateResources& resources,
        std::chrono::duration<std::chrono::high_resolution_clock::rep,
        std::chrono::high_resolution_clock::period> timespan)
        : _asyncTimer(resources.asyncTimerFactory.createAsyncTimer(timespan, std::bind(&RunningState::_dequeueAndSendAll, this))),
          _queue(),
          _resources(resources)
    {
        _asyncTimer->start();
    }

    /*
     * _dequeueAndSendAll() is called at every interval as set on the IAsyncTimer
     * (which will be the length of the initial loop) until the state
     * changes.
     */
    void RunningState::_dequeueAndSendAll()
    {
        std::unique_ptr<StartMessage> message;
        while (_queue.tryDequeue(message))
        {
            _resources.messageDispatcher.sendMidiMessage(message.get());
            // inform the loopTracker that a command has been received
            _resources.loopTracker.startCommand(message->getCommand());
        }

        // Get vector of restart messages to send
        for (auto& restartCommand : _resources.loopTracker.getNextRestartCommands())
        {
            _resources.messageDispatcher.sendNewMidiMessage(restartCommand);
        }

        _resources.loopTracker.incrementInterval();
    }

    void RunningState::handle(std::unique_ptr<StateBase>& state, std::unique_ptr<StartMessage> message)
    {
        _queue.enqueue(std::move(message));
    }

    void RunningState::handle(std::unique_ptr<StateBase>& state, std::unique_ptr<StopMessage> message)
    {
        _resources.messageDispatcher.sendMidiMessage(message.get());
        _resources.loopTracker.stopCommand(message->getStartCommand());
    }

    void RunningState::handleStdin(std::unique_ptr<StateBase>& state, const std::string& input)
    {
        _resources.loopTracker.clear();
        _resources.logger.log(std::make_unique<StateChangedEvent>(
            std::string("StdIn. Reset: Running -> Created"),
            std::string("RunningState")));
        state = std::make_unique<CreatedState>(_resources);
    }

}
