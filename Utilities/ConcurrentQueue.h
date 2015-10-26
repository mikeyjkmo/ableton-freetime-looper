#pragma once

#include <queue>
#include <memory>
#include <mutex>

namespace AbletonProject
{
    template<typename T>
    class ConcurrentQueue final
    {
    private:
        std::queue<T> _queue;
        std::mutex _mutex;

    public:
        ConcurrentQueue() : _queue(), _mutex()
        {
        }

        bool empty() const
        {
            return _queue.empty();
        }

        std::size_t size() const
        {
            return _queue.size();
        }

        void enqueue(T item)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(std::move(item));
        }

        bool tryDequeue(T& item)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            if (_queue.empty()) return false;

            item = std::move(_queue.front());
            _queue.pop();
            return true;
        }
    };
}
