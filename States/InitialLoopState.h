#pragma once
#include <string>

#include "StateBase.h"
#include "Stopwatch.h"
#include "MessageDispatcher.h"

namespace AbletonProject
{
	// Waiting for and timing completion of intial loop
	class InitialLoopState : public StateBase
	{
	private:
		Stopwatch _stopWatch;
		MessageDispatcher* _messageDispatcher;
	public:
		InitialLoopState(MessageDispatcher* messageDispatcher);
		virtual ~InitialLoopState() = default;

		void handle(std::unique_ptr<StateBase>& state, std::string message);
	};

}