#pragma once
#include <string>
#include <chrono>

#include "StateBase.h"
#include "Timer.h"
#include "ConcurrentQueue.h"
#include "MessageDispatcher.h"

namespace AbletonProject
{

	class RunningState : public StateBase
	{
	private:
		Timer _timer;
		ConcurrentQueue<std::string> _queue;
		void forwardMessage();
		MessageDispatcher* _messageDispatcher;
	public:
		RunningState(std::chrono::milliseconds timespan, MessageDispatcher* messageDispatcher);
		virtual ~RunningState() = default;

		void dequeueAndSendAll();
		void handle(std::unique_ptr<StateBase>& state, std::string message);
	};

}