#pragma once
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

class NotificationDispatcher {
public:
    static NotificationDispatcher& instance();

    // Post a task to be executed asynchronously by dispatcher thread.
    void post(std::function<void()> task);

    // Stop the dispatcher (called in destructor)
    void stop();

private:
    NotificationDispatcher();
    ~NotificationDispatcher();

    void run();

    std::thread worker_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> tasks_;
    bool running_ = true;
};
