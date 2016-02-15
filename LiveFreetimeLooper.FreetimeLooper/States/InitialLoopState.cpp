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
        _resources.messageDispatcher.sendMidiMessage(message.get());

        if (_loopStartingMessage == *message)
        {
            _stopWatch.stop();
            _resources.loopTracker.incrementInterval();
            _resources.loopTracker.commandReceived(message->command);
            _resources.logger.log(std::make_unique<StateChangedEvent>(
                std::string("End Message received. Interval measured at ") +
                std::to_string(_stopWatch.getElapsedMilliseconds().count()) +
                std::string("ms. Progress: InitialLoop -> Running"),
                std::string("InitialLoopState")));
            state = std::move(std::make_unique<RunningState>(
                _resources, _stopWatch.getElapsed()));
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
