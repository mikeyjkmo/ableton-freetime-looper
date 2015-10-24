#include "Stopwatch.h"

namespace AbletonProject
{
	Stopwatch::Stopwatch() :
		_isRunning(false),
		_elapsed(std::chrono::system_clock::duration::zero()),
		_startTime(std::chrono::system_clock::duration::zero())
	{
	}

	void Stopwatch::start()
	{
		if (_isRunning) return;

		_startTime = std::chrono::high_resolution_clock::now();
		_isRunning = true;
	}

	void Stopwatch::stop()
	{
		if (!_isRunning) return;
		_elapsed = calculateElapsed();
		_isRunning = false;
	}

	void Stopwatch::reset()
	{
		_elapsed = std::chrono::system_clock::duration::zero();
		_isRunning = false;
	}

	void Stopwatch::restart()
	{
		reset();
		start();
	}

	bool Stopwatch::IsRunning()
	{
		return _isRunning;
	}
	
	std::chrono::milliseconds Stopwatch::getElapsedMilliseconds() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(getElapsed());
	}

	std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> Stopwatch::getElapsed() const
	{
		if (!_isRunning) return _elapsed;
		return calculateElapsed();
	}

	std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> Stopwatch::calculateElapsed() const
	{
		return _elapsed + std::chrono::high_resolution_clock::now() - _startTime;
	}
}