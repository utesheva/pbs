#include "ClientObserver.hpp"
#include "Observer.hpp"
#include "Order.hpp"

#include <mutex>

class LoggerObserver;

ClientObserver::ClientObserver(int orderId)
    : orderId_(orderId) {
}

void ClientObserver::Update(const Order& order) {
    std::lock_guard<std::mutex> lk(mtx_);
    lastStatus_ = order.StatusString();
    cv_.notify_all();
}

bool ClientObserver::WaitForStatusChange(int timeout_ms = 100) {
    std::unique_lock<std::mutex> lk(mtx_);
    if (timeout_ms == 0) {
        cv_.wait(lk);
        return true;
    } else {
        return cv_.wait_for(lk, std::chrono::milliseconds(timeout_ms)) !=
               std::cv_status::timeout;
    }
}

std::string ClientObserver::LastStatus() const {
    std::lock_guard<std::mutex> lk(mtx_);
    return lastStatus_;
}
