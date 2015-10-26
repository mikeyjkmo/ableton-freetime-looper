#pragma once

#include <chrono>

namespace AbletonProject
{

    class Stopwatch final
    {
    private:
        bool _isRunning;
        std::chrono::high_resolution_clock::time_point _startTime;
        std::chrono::duration<std::chrono::high_resolution_clock::rep,
                              std::chrono::high_resolution_clock::period> _elapsed;
        std::chrono::duration<std::chrono::high_resolution_clock::rep,
                              std::chrono::high_resolution_clock::period> calculateElapsed() const;
        std::chrono::duration<std::chrono::high_resolution_clock::rep,
                              std::chrono::high_resolution_clock::period> getElapsed() const;
    public:
        Stopwatch();

        void start();
        void stop();
        void reset();
        void restart();
        bool IsRunning();
        std::chrono::milliseconds getElapsedMilliseconds() const;
    };
}
