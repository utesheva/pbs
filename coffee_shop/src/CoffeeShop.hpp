#pragma once

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "Order.hpp"

class CoffeeShop {
 public:
  CoffeeShop();
  ~CoffeeShop();

  void Start(size_t num_baristas);
  void Stop();

  void SubmitOrder(std::shared_ptr<Order> order);
  bool CancelOrder(int orderId);

 private:
  void BaristaLoop(int idx);

  std::deque<std::shared_ptr<Order>> queue_;
  std::mutex queue_mtx_;
  std::condition_variable cv_;
  bool shutdown_ = false;
  std::vector<std::thread> baristas_;
};
