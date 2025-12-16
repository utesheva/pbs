#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

class NotificationDispatcher {
public:
    static NotificationDispatcher& Instance();

    void Post(std::function<void()> task);

    void Stop();

private:
    NotificationDispatcher();
    ~NotificationDispatcher();

    void Run();

    std::thread worker_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> tasks_;
    bool running_ = true;
};
