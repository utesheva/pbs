#include "CoffeeShop.hpp"

#include <chrono>
#include <thread>

CoffeeShop::CoffeeShop() = default;

CoffeeShop::~CoffeeShop() { Stop(); }

void CoffeeShop::Start(size_t num_baristas) {
  shutdown_ = false;
  for (size_t i = 0; i < num_baristas; ++i) {
    baristas_.emplace_back([this, i]() { BaristaLoop(static_cast<int>(i)); });
  }
}

void CoffeeShop::Stop() {
  {
    std::lock_guard<std::mutex> lk(queue_mtx_);
    shutdown_ = true;
  }
  cv_.notify_all();
  for (auto& t : baristas_) {
    if (t.joinable()) {
      t.join();
    }
  }
  baristas_.clear();
}

void CoffeeShop::SubmitOrder(std::shared_ptr<Order> order) {
  {
    std::lock_guard<std::mutex> lk(queue_mtx_);
    queue_.push_back(order);
  }
  cv_.notify_one();
}

bool CoffeeShop::CancelOrder(int orderId) {
  std::lock_guard<std::mutex> lk(queue_mtx_);
  for (auto it = queue_.begin(); it != queue_.end(); ++it) {
    if ((*it)->Id() == orderId) {
      auto order = *it;
      queue_.erase(it);
      order->SetStatus(Order::StatusType::Cancelled);
      return true;
    }
  }
  return false;
}

void CoffeeShop::BaristaLoop(int idx) {
  while (true) {
    std::shared_ptr<Order> order;
    {
      std::unique_lock<std::mutex> lk(queue_mtx_);
      cv_.wait(lk, [this]() { return shutdown_ || !queue_.empty(); });
      if (shutdown_ && queue_.empty()) {
        return;
      }
      if (!queue_.empty()) {
        order = queue_.front();
        queue_.pop_front();
      } else {
        continue;
      }
    }

    if (order->Status() == Order::StatusType::Cancelled) {
      continue;
    }

    if (order->Status() == Order::StatusType::Placed) {
      order->SetStatus(Order::StatusType::Preparing);
      int total = order->PrepareTime();
      for (int s = 0; s < total; ++s) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (order->Status() == Order::StatusType::Cancelled) {
          break;
        }
      }
      if (order->Status() == Order::StatusType::Cancelled) {
        continue;
      }
      order->SetStatus(Order::StatusType::Ready);
      order->SetStatus(Order::StatusType::Completed);
    } else {
      continue;
    }
  }
}
