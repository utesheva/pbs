#include "CoffeeShop.hpp"
#include <iostream>
#include <chrono>
#include <thread>

CoffeeShop::CoffeeShop() = default;

CoffeeShop::~CoffeeShop() {
    stop();
}

void CoffeeShop::start(size_t num_baristas) {
    shutdown_ = false;
    for (size_t i = 0; i < num_baristas; ++i) {
        baristas_.emplace_back([this, i](){ baristaLoop(static_cast<int>(i)); });
    }
}

void CoffeeShop::stop() {
    {
        std::lock_guard<std::mutex> lk(queue_mtx_);
        shutdown_ = true;
    }
    cv_.notify_all();
    for (auto& t : baristas_) {
        if (t.joinable()) t.join();
    }
    baristas_.clear();
}

void CoffeeShop::submitOrder(std::shared_ptr<Order> order) {
    {
        std::lock_guard<std::mutex> lk(queue_mtx_);
        queue_.push_back(order);
    }
    cv_.notify_one();
}

bool CoffeeShop::cancelOrder(int orderId) {
    std::lock_guard<std::mutex> lk(queue_mtx_);
    for (auto it = queue_.begin(); it != queue_.end(); ++it) {
        if ((*it)->id() == orderId) {
            auto order = *it;
            queue_.erase(it);
            order->setStatus(Order::Status::Cancelled);
            return true;
        }
    }
    // not found in queue => already taken by barista or unknown
    return false;
}

void CoffeeShop::baristaLoop(int idx) {
    while (true) {
        std::shared_ptr<Order> order;
        {
            std::unique_lock<std::mutex> lk(queue_mtx_);
            cv_.wait(lk, [this](){ return shutdown_ || !queue_.empty(); });
            if (shutdown_ && queue_.empty()) return;
            if (!queue_.empty()) {
                order = queue_.front();
                queue_.pop_front();
            } else { continue; }
        }

        if (order->status() == Order::Status::Cancelled) {
            continue;
        }

        // Mark preparing only if still placed
        if (order->status() == Order::Status::Placed) {
            order->setStatus(Order::Status::Preparing);
            // Simulate prepare - but allow early cancellation: periodically check status
            int total = order->prepareTime();
            for (int s = 0; s < total; ++s) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                if (order->status() == Order::Status::Cancelled) break;
            }
            if (order->status() == Order::Status::Cancelled) {
                // skip finishing
                continue;
            }
            order->setStatus(Order::Status::Ready);
            // Simulate completion step
            order->setStatus(Order::Status::Completed);
        } else {
            // If someone changed status meanwhile (e.g. cancelled) skip
        }
    }
}
