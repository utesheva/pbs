#pragma once
#include <memory>
#include <vector>
#include <mutex>
#include <string>
#include <atomic>
#include "Drinks.hpp"
#include "Observer.hpp"

class PaymentStrategy;

class Order: public std::enable_shared_from_this<Order> {
public:
    enum class Status { Placed, Preparing, Ready, Completed, Paid, Cancelled };

    Order(int id, std::unique_ptr<Drink> drink, std::unique_ptr<PaymentStrategy> payment);

    int id() const;
    std::string drinkName() const;
    int prepareTime() const;

    Status status() const;
    std::string statusString() const;

    void setStatus(Status s);
    bool pay(std::string& receipt);

    void attach(std::shared_ptr<IObserver> obs);
    void detach(std::shared_ptr<IObserver> obs);

private:
    void notify() const;

    int id_;
    std::unique_ptr<Drink> drink_;
    std::unique_ptr<PaymentStrategy> payment_;
    mutable std::mutex mtx_;
    Status status_;
    std::vector<std::weak_ptr<IObserver>> observers_;
};
