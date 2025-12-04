#pragma once
#include <memory>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include "Order.hpp"
#include "CoffeeShop.hpp"
#include "Observer.hpp"

class LoggerObserver;

class OrderManager {
public:
    OrderManager(CoffeeShop& shop, std::shared_ptr<LoggerObserver> logger);
    ~OrderManager();

    // Create and submit order. Attaches logger automatically. Returns shared_ptr<Order>.
    // Throws std::invalid_argument if drink/payment unknown.
    std::shared_ptr<Order> createOrder(const std::string& drinkType, const std::string& paymentType);

    // Get order by id (returns nullptr if not found)
    std::shared_ptr<Order> getOrder(int id);

    // Pay order by id. Returns pair(success, receipt).
    std::pair<bool, std::string> payOrder(int id);

private:
    CoffeeShop& shop_;
    std::shared_ptr<LoggerObserver> logger_;
    std::atomic<int> nextId_{1};
    std::unordered_map<int, std::shared_ptr<Order>> orders_;
    std::mutex mtx_;
};
