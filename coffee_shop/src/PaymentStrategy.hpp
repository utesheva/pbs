#pragma once
#include <memory>
#include <string>

class Order;

class PaymentStrategy {
 public:
  virtual ~PaymentStrategy() = default;
  virtual bool Pay(const Order& order, std::string& out_receipt) = 0;
  virtual std::string Name() const = 0;
};

std::unique_ptr<PaymentStrategy> MakeCashPayment();
std::unique_ptr<PaymentStrategy> MakeCreditCardPayment();
