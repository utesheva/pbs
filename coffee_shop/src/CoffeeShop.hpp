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

    void start(size_t num_baristas);
    void stop();

    void submitOrder(std::shared_ptr<Order> order);
    bool cancelOrder(int orderId);

private:
    void baristaLoop(int idx);

    std::deque<std::shared_ptr<Order>> queue_;
    std::mutex queue_mtx_;
    std::condition_variable cv_;
    bool shutdown_ = false;
    std::vector<std::thread> baristas_;
};
