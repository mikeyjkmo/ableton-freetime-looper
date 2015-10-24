#pragma once

#include <string>

#include <iostream>

namespace AbletonProject
{

	class MessageDispatcher
	{
	public:
		MessageDispatcher();
		virtual ~MessageDispatcher() = default;
		
		void sendMessage(std::string message);
	};

}