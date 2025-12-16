#include "NotificationDispatcher.hpp"

#include <utility>

NotificationDispatcher& NotificationDispatcher::Instance() {
  static NotificationDispatcher inst;
  return inst;
}

NotificationDispatcher::NotificationDispatcher() {
  worker_ = std::thread([this]() { Run(); });
}

NotificationDispatcher::~NotificationDispatcher() { Stop(); }

void NotificationDispatcher::Stop() {
  {
    std::lock_guard<std::mutex> lk(mtx_);
    if (!running_) {
      return;
    }
    running_ = false;
  }
  cv_.notify_all();
  if (worker_.joinable()) {
    worker_.join();
  }
}

void NotificationDispatcher::Post(std::function<void()> task) {
  {
    std::lock_guard<std::mutex> lk(mtx_);
    tasks_.push(std::move(task));
  }
  cv_.notify_one();
}

void NotificationDispatcher::Run() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lk(mtx_);
      cv_.wait(lk, [this]() { return !tasks_.empty() || !running_; });
      if (!running_ && tasks_.empty()) {
        return;
      }
      task = std::move(tasks_.front());
      tasks_.pop();
    }
    try {
      if (task) {
        task();
      }
    } catch (...) {
      // swallow exceptions: observers shouldn't bring server down
    }
  }
}
