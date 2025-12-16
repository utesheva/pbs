#pragma once
#include <condition_variable>
#include <mutex>
#include <string>

#include "Observer.hpp"
#include "Order.hpp"

class ClientObserver: public IObserver,
                      public std::enable_shared_from_this<ClientObserver> {
public:
    explicit ClientObserver(int orderId);

    void update(const Order& order) override;

    bool waitForStatusChange(int timeout_ms);

    std::string lastStatus() const;

private:
    int orderId_;
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    std::string lastStatus_;
};
