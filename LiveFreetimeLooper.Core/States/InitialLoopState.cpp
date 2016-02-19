#include "InitialLoopState.hpp"
#include "RunningState.hpp"
#include "CreatedState.hpp"
#include "../Logging/StateChangedEvent.hpp"

namespace LiveFreetimeLooper
{

    InitialLoopState::InitialLoopState(StateResources& resources, StartMessage& message)
        : _stopWatch(),
         _resources(resources),
         _loopStartingMessage(message)
    {
        _stopWatch.start();
    }

    void InitialLoopState::handle(std::unique_ptr<StateBase>& state, std::unique_ptr<StartMessage> message)
    {
        if (_loopStartingMessage == *message)
        {
            _resources.messageDispatcher.sendMidiMessage(message.get());

            _stopWatch.stop();
            _resources.loopTracker.incrementInterval();
            _resources.loopTracker.startCommand(message->getCommand());
            _resources.logger.log(std::make_unique<StateChangedEvent>(
                std::string("End Message received. Interval measured at ") +
                std::to_string(_stopWatch.getElapsedMilliseconds().count()) +
                std::string("ms. Progress: InitialLoop -> Running"),
                std::string("InitialLoopState")));
            state = std::move(std::make_unique<RunningState>(
                _resources, _stopWatch.getElapsed()));
        }
        else
        {
            // ignore
        }
    }

    void InitialLoopState::handle(std::unique_ptr<StateBase>& state, std::unique_ptr<StopMessage> message)
    {
        if (message->isMatchingStopMessageFor(_loopStartingMessage))
        {
            // not implemented
        }
        else
        {
            _resources.messageDispatcher.sendMidiMessage(message.get());
        }
    }

    void InitialLoopState::handleStdin(std::unique_ptr<StateBase>& state, const std::string& input)
    {
        _resources.loopTracker.clear();
        _resources.logger.log(std::make_unique<StateChangedEvent>(
            std::string("StdIn. Reset: InitialLoop -> Created"),
            std::string("InitialLoopState")));
        state = std::move(std::make_unique<CreatedState>(_resources));
    }
}
