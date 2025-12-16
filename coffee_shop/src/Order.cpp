#include "Order.hpp"
#include "NotificationDispatcher.hpp"
#include "PaymentStrategy.hpp"

#include <algorithm>

Order::Order(int id, std::unique_ptr<Drink> drink,
             std::unique_ptr<PaymentStrategy> payment)
    : id_(id)
    , drink_(std::move(drink))
    , payment_(std::move(payment))
    , status_(StatusType::Placed) {
}

int Order::Id() const {
    return id_;
}
std::string Order::DrinkName() const {
    return drink_->Name();
}
int Order::PrepareTime() const {
    return drink_->PrepareTimeSeconds();
}

Order::Status Order::Status() const {
    std::lock_guard<std::mutex> lk(mtx_);
    return status_;
}

std::string Order::StatusString() const {
    switch (Status()) {
        case StatusType::Placed:
            return "Placed";
        case StatusType::Preparing:
            return "Preparing";
        case StatusType::Ready:
            return "Ready";
        case StatusType::Completed:
            return "Completed";
        case StatusType::Paid:
            return "Paid";
        case StatusType::Cancelled:
            return "Cancelled";
    }
    return "Unknown";
}

void Order::SetStatus(Status s) {
    {
        std::lock_guard<std::mutex> lk(mtx_);
        status_ = s;
    }
    Notify();
}

bool Order::Pay(std::string& receipt) {
    if (!payment_) {
        return false;
    }
    bool ok = payment_->Pay(*this, receipt);
    if (ok) {
        SetStatus(StatusType::Paid);
    }
    return ok;
}

void Order::Attach(std::shared_ptr<IObserver> obs) {
    std::lock_guard<std::mutex> lk(mtx_);
    observers_.push_back(obs);
}

void Order::Detach(std::shared_ptr<IObserver> obs) {
    std::lock_guard<std::mutex> lk(mtx_);
    observers_.erase(std::remove_if(observers_.begin(), observers_.end(),
                                    [&](const std::weak_ptr<IObserver>& w) {
                                        auto s = w.lock();
                                        return !s || s == obs;
                                    }),
                     observers_.end());
}

void Order::Notify() const {
    std::vector<std::shared_ptr<IObserver>> listeners;
    {
        std::lock_guard<std::mutex> lk(mtx_);
        for (auto& w : observers_) {
            if (auto s = w.lock()) {
                listeners.push_back(s);
            }
        }
    }
    auto self = const_cast<Order*>(this)->shared_from_this();
    for (auto& l : listeners) {
        auto observer = l;
        NotificationDispatcher::Instance().Post([observer, self]() {
            try {
                observer->Update(*self);
            } catch (...) {
                // swallow to avoid crashing dispatcher
            }
        });
    }
}
