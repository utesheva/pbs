#include "Order.hpp"
#include <algorithm>
#include "NotificationDispatcher.hpp"
#include "PaymentStrategy.hpp"

Order::Order(int id, std::unique_ptr<Drink> drink,
             std::unique_ptr<PaymentStrategy> payment)
    : id_(id),
      drink_(std::move(drink)),
      payment_(std::move(payment)),
      status_(Status::Placed) {}

int Order::id() const { return id_; }
std::string Order::drinkName() const { return drink_->name(); }
int Order::prepareTime() const { return drink_->prepareTimeSeconds(); }

Order::Status Order::status() const {
  std::lock_guard<std::mutex> lk(mtx_);
  return status_;
}

std::string Order::statusString() const {
  switch (status()) {
    case Status::Placed:
      return "Placed";
    case Status::Preparing:
      return "Preparing";
    case Status::Ready:
      return "Ready";
    case Status::Completed:
      return "Completed";
    case Status::Paid:
      return "Paid";
    case Status::Cancelled:
      return "Cancelled";
  }
  return "Unknown";
}

void Order::setStatus(Status s) {
  {
    std::lock_guard<std::mutex> lk(mtx_);
    status_ = s;
  }
  notify();
}

bool Order::pay(std::string& receipt) {
  if (!payment_) return false;
  bool ok = payment_->pay(*this, receipt);
  if (ok) {
    setStatus(Status::Paid);
  }
  return ok;
}

void Order::attach(std::shared_ptr<IObserver> obs) {
  std::lock_guard<std::mutex> lk(mtx_);
  observers_.push_back(obs);
}

void Order::detach(std::shared_ptr<IObserver> obs) {
  std::lock_guard<std::mutex> lk(mtx_);
  observers_.erase(std::remove_if(observers_.begin(), observers_.end(),
                                  [&](const std::weak_ptr<IObserver>& w) {
                                    auto s = w.lock();
                                    return !s || s == obs;
                                  }),
                   observers_.end());
}

void Order::notify() const {
  std::vector<std::shared_ptr<IObserver>> listeners;
  {
    std::lock_guard<std::mutex> lk(mtx_);
    for (auto& w : observers_) {
      if (auto s = w.lock()) listeners.push_back(s);
    }
  }
  auto self = const_cast<Order*>(this)->shared_from_this();
  for (auto& l : listeners) {
    auto observer = l;
    NotificationDispatcher::instance().post([observer, self]() {
      try {
        observer->update(*self);
      } catch (...) {
        // swallow to avoid crashing dispatcher
      }
    });
  }
}
