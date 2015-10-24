#include "RunningState.h"

#include <stdexcept>

namespace AbletonProject
{

	RunningState::RunningState(std::chrono::milliseconds timespan, MessageDispatcher* messageDispatcher)
		: _timer(timespan),
		_queue(),
		_messageDispatcher(messageDispatcher)
	{
	}

	void RunningState::dequeueAndSendAll()
	{
		std::string message;
		while (_queue.tryDequeue(message))
		{
			_messageDispatcher->sendMessage(message);
		}
	}

	void RunningState::handle(std::unique_ptr<StateBase>& state, std::string message)
	{
		_queue.enqueue(message);

		// todo for now immeditetly forward
		dequeueAndSendAll();
	}
}