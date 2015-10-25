#include "CreatedState.h"

namespace AbletonProject
{

    CreatedState::CreatedState(MessageDispatcher& messageDispatcher, EventLogger& logger) :
        _messageDispatcher(messageDispatcher),
        _logger(logger)
    {
    }

    void CreatedState::handle(std::unique_ptr<StateBase>& state, Message message)
    {
        _messageDispatcher.sendMidiMessage(message);
    }

    void CreatedState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::make_unique<InitialLoopWaitingState>(_messageDispatcher, _logger);
        _logger.Log(std::make_unique<StateChangedEvent>(
            std::string("StdIn detected, moving from Created to InitialLoopWaiting"),
            std::string("CreatedState")));
    }

}
