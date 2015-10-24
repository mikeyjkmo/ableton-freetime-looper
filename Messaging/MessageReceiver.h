#pragma once
#include <memory>
#include <string>
#include <mutex>

#include "StateBase.h"
#include "CreatedState.h"
#include "MessageDispatcher.h"

namespace AbletonProject
{
	class MessageReceiver
	{
	private:
		std::unique_ptr<StateBase> _currentState;
		std::unique_ptr<MessageDispatcher> _messageDispatcher;
		std::mutex _mutex;
	public:
		MessageReceiver();
		virtual ~MessageReceiver() = default;

		void receiveMessage(std::string message);
	};

}