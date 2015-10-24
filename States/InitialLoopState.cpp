#include "InitialLoopState.h"

namespace AbletonProject
{

    InitialLoopState::InitialLoopState(MessageDispatcher& messageDispatcher) :
        _stopWatch(),
        _messageDispatcher(messageDispatcher)
    {
        _stopWatch.start();
    }

    void InitialLoopState::handle(std::unique_ptr<StateBase>& state, Message& message)
    {
        _stopWatch.stop();

        state = std::move(std::make_unique<RunningState>(_stopWatch.getElapsedMilliseconds(), _messageDispatcher));
        _messageDispatcher.sendMessage(message);
    }

    void InitialLoopState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::move(std::make_unique<CreatedState>(_messageDispatcher));
    }
}
