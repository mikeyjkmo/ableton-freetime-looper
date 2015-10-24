#pragma once
#include <memory>
#include <string>

#include "Stopwatch.h"
#include "StateBase.h"
#include "MessageDispatcher.h"

namespace AbletonProject
{
	// Waiting for first message to start Initial Loop
	class CreatedState : public StateBase
	{
	private:
		MessageDispatcher* _messageDispatcher;
	public:
		CreatedState(MessageDispatcher* messageDispatcher);
	    virtual ~CreatedState() = default;

		void handle(std::unique_ptr<StateBase>& state, std::string message);
	};

}