#pragma once
#include <memory>
#include <string>

class Order;

class PaymentStrategy {
 public:
  virtual ~PaymentStrategy() = default;
  virtual bool pay(const Order& order, std::string& out_receipt) = 0;
  virtual std::string name() const = 0;
};

std::unique_ptr<PaymentStrategy> makeCashPayment();
std::unique_ptr<PaymentStrategy> makeCreditCardPayment();
