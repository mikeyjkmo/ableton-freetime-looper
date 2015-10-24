#pragma once
#include <chrono>

namespace AbletonProject
{

	class Timer
	{
	public:
		Timer(std::chrono::milliseconds duration);
		virtual ~Timer() = default;

		void start();
		void stop();
	};

}