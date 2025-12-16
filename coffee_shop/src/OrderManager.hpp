#pragma once
#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "CoffeeShop.hpp"
#include "Observer.hpp"
#include "Order.hpp"

class LoggerObserver;

class OrderManager {
 public:
  OrderManager(CoffeeShop& shop, std::shared_ptr<LoggerObserver> logger);
  ~OrderManager();

  std::shared_ptr<Order> CreateOrder(const std::string& drinkType,
                                     const std::string& paymentType);

  std::shared_ptr<Order> GetOrder(int id);

  std::pair<bool, std::string> PayOrder(int id);

 private:
  CoffeeShop& shop_;
  std::shared_ptr<LoggerObserver> logger_;
  std::atomic<int> nextId_{1};
  std::unordered_map<int, std::shared_ptr<Order>> orders_;
  std::mutex mtx_;
};
