#include "RunningState.h"

namespace AbletonProject
{

    RunningState::RunningState(StateResources& resources, std::chrono::milliseconds timespan)
        : _asyncTimer(timespan, std::bind(&RunningState::_dequeueAndSendAll, this)),
          _queue(),
          _resources(resources)
    {
        _asyncTimer.start();
    }

    void RunningState::_dequeueAndSendAll()
    {
        std::unique_ptr<Message> message;
        while (_queue.tryDequeue(message))
        {
            _resources.messageDispatcher.sendMidiMessage(message.get());
        }
    }

    void RunningState::handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message)
    {
        _queue.enqueue(std::move(message));
    }

    void RunningState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::make_unique<CreatedState>(_resources);
        _resources.logger.log(std::make_unique<StateChangedEvent>(
            std::string("StdIn detected, reverting from Running to Created"),
            std::string("RunningState")));
    }

}
