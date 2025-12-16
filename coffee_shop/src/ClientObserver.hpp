#pragma once
#include <condition_variable>
#include <mutex>
#include <string>

#include "Observer.hpp"
#include "Order.hpp"

class ClientObserver : public IObserver,
                       public std::enable_shared_from_this<ClientObserver> {
 public:
  explicit ClientObserver(int orderId);

  void Update(const Order& order) override;

  bool WaitForStatusChange(int timeout_ms);

  std::string LastStatus() const;

 private:
  int orderId_;
  mutable std::mutex mtx_;
  std::condition_variable cv_;
  std::string lastStatus_;
};
