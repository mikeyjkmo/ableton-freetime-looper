#include "InitialLoopState.h"

namespace AbletonProject
{

    InitialLoopState::InitialLoopState(MessageDispatcher& messageDispatcher, EventLogger& logger, Message message) :
        _stopWatch(),
        _messageDispatcher(messageDispatcher),
        _loopStartingMessage(message),
        _logger(logger)
    {
        _stopWatch.start();
    }

    void InitialLoopState::handle(std::unique_ptr<StateBase>& state, Message message)
    {
        _messageDispatcher.sendMidiMessage(message);

        if (_loopStartingMessage == message)
        {
            _stopWatch.stop();
            state = std::move(std::make_unique<RunningState>(
                _stopWatch.getElapsedMilliseconds(), _messageDispatcher, _logger));
            _logger.Log(std::make_unique<StateChangedEvent>(
                std::string("Loop ending message detected, moving from InitialLoop to Running"),
                std::string("InitialLoopState")));
        }
    }

    void InitialLoopState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::move(std::make_unique<CreatedState>(_messageDispatcher, _logger));
        _logger.Log(std::make_unique<StateChangedEvent>(
            std::string("StdIn detected, reverting from InitialLoop to Created"),
            std::string("InitialLoopState")));
    }
}
