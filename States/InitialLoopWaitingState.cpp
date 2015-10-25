#include "InitialLoopWaitingState.h"

namespace AbletonProject
{

    InitialLoopWaitingState::InitialLoopWaitingState(MessageDispatcher& messageDispatcher, EventLogger& logger) :
        _messageDispatcher(messageDispatcher),
        _logger(logger)
    {
    }

    void InitialLoopWaitingState::handle(std::unique_ptr<StateBase>& state, Message message)
    {
        state = std::make_unique<InitialLoopState>(_messageDispatcher, _logger, message);
        _logger.Log(std::make_unique<StateChangedEvent>(
            std::string("Loop starting message detected, moving from InitialLoopWaiting to InitialLoop"),
            std::string("InitialLoopWaitingState")));
        _messageDispatcher.sendMidiMessage(message);
    }

    void InitialLoopWaitingState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::move(std::make_unique<CreatedState>(_messageDispatcher, _logger));
        _logger.Log(std::make_unique<StateChangedEvent>(
            std::string("StdIn detected, reverting from InitialLoopWaiting to Created"),
            std::string("InitialLoopWaitingState")));
    }

}
