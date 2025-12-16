#pragma once

class Order;

class IObserver {
 public:
  virtual ~IObserver() = default;
  virtual void Update(const Order& order) = 0;
};
