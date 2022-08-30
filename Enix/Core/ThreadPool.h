//
// Created by Energy on 2022/08/31.
//

#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>


// Thread pool reference: https://github.com/progschj/ThreadPool
class ThreadPool {
public:
    explicit ThreadPool(size_t threadCount = std::thread::hardware_concurrency() - 1) {
        _workerThreads.reserve(threadCount);
        for (size_t i = 0; i < threadCount; i++) {
            _workerThreads.emplace_back(
                    [this] {
                        while (true) {
                            std::packaged_task<void()> task;
                            {
                                std::unique_lock<std::mutex> lock(this->_queueMutex);
                                this->_queueConditionVariable.wait(lock, [&] {
                                    return this->_shouldStop || !this->_taskQueue.empty();
                                });
                                if (this->_shouldStop && this->_taskQueue.empty())
                                    return;
                                task = std::move(this->_taskQueue.front());
                                this->_taskQueue.pop();
                            }
                            task();
                        }
                    });
        }
    }

    ~ThreadPool() {
        _shouldStop = true;
        _queueConditionVariable.notify_all();
        for (auto &worker: _workerThreads) { worker.join(); }
    }

    template<class F, class... Args>
    auto enqueue(F &&f, Args &&... args) {
        using returnType = std::invoke_result_t<F, Args...>;
        std::packaged_task<returnType()> task{[f = std::forward<F>(f), ... args = std::forward<Args>(args)]() mutable {
            return f(std::forward<Args>(args)...);
        }};
        auto result = task.get_future();
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            if (_shouldStop) {
                throw std::runtime_error("Cannot enqueue on stopped thread pool!");
            }
            _taskQueue.emplace(std::move(task));
        }
        _queueConditionVariable.notify_one();
        return result;
    }

private:
    std::atomic<bool> _shouldStop{false};
    std::vector<std::jthread> _workerThreads;
    std::queue<std::packaged_task<void()>> _taskQueue;
    std::mutex _queueMutex;
    std::condition_variable _queueConditionVariable;
};