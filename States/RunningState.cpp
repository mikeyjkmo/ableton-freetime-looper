#include "RunningState.h"

namespace AbletonProject
{

    RunningState::RunningState(std::chrono::milliseconds timespan, MessageDispatcher& messageDispatcher, EventLogger& logger)
        : _asyncTimer(timespan, std::bind(&RunningState::_dequeueAndSendAll, this)),
          _queue(),
          _messageDispatcher(messageDispatcher),
          _logger(logger)
    {
        _asyncTimer.start();
    }

    void RunningState::_dequeueAndSendAll()
    {
        std::unique_ptr<Message> message;
        while (_queue.tryDequeue(message))
        {
            _messageDispatcher.sendMidiMessage(message.get());
        }
    }

    void RunningState::handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message)
    {
        _queue.enqueue(std::move(message));
    }

    void RunningState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::make_unique<CreatedState>(_messageDispatcher, _logger);
        _logger.Log(std::make_unique<StateChangedEvent>(
            std::string("StdIn detected, reverting from Running to Created"),
            std::string("RunningState")));
    }

}
