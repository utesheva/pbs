#pragma once
#include <string>
#include <memory>

class Order; // forward

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    // Return true if payment succeeded
    virtual bool pay(const Order& order, std::string& out_receipt) = 0;
    virtual std::string name() const = 0;
};

// Factory helper declarations.
// Implementations are in src/PaymentStrategy.cpp
std::unique_ptr<PaymentStrategy> makeCashPayment();
std::unique_ptr<PaymentStrategy> makeCreditCardPayment();
