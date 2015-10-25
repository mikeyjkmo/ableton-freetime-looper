#include "InitialLoopState.h"

namespace AbletonProject
{

    InitialLoopState::InitialLoopState(MessageDispatcher& messageDispatcher, Message message) :
        _stopWatch(),
        _messageDispatcher(messageDispatcher),
        _loopStartingMessage(message)
    {
        _stopWatch.start();
    }

    void InitialLoopState::handle(std::unique_ptr<StateBase>& state, Message& message)
    {
        _messageDispatcher.sendMessage(message);

        if (!_loopStartingMessage.Matches(message)) return;

        _stopWatch.stop();
        state = std::move(std::make_unique<RunningState>(_stopWatch.getElapsedMilliseconds(), _messageDispatcher));
    }

    void InitialLoopState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::move(std::make_unique<CreatedState>(_messageDispatcher));
    }
}
