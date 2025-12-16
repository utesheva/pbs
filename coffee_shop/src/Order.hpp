#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "Drinks.hpp"
#include "Observer.hpp"

class PaymentStrategy;

class Order : public std::enable_shared_from_this<Order> {
 public:
  enum class StatusType {
    Placed,
    Preparing,
    Ready,
    Completed,
    Paid,
    Cancelled
  };

  Order(int id, std::unique_ptr<Drink> drink,
        std::unique_ptr<PaymentStrategy> payment);

  int Id() const;
  std::string DrinkName() const;
  int PrepareTime() const;

  StatusType Status() const;
  std::string StatusString() const;

  void SetStatus(StatusType s);
  bool Pay(std::string& receipt);

  void Attach(std::shared_ptr<IObserver> obs);
  void Detach(std::shared_ptr<IObserver> obs);

 private:
  void Notify() const;

  int id_;
  std::unique_ptr<Drink> drink_;
  std::unique_ptr<PaymentStrategy> payment_;
  mutable std::mutex mtx_;
  StatusType status_;
  std::vector<std::weak_ptr<IObserver>> observers_;
};
