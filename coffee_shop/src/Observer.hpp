#pragma once

class Order;

class IObserver {
 public:
  virtual ~IObserver() = default;
  virtual void update(const Order& order) = 0;
};
