#include "PaymentStrategy.hpp"
#include <string>
#include "Order.hpp"
#include <memory>

// Simple strategies implemented here for demo
class CashPayment : public PaymentStrategy {
public:
    bool pay(const Order&, std::string& out_receipt) override {
        out_receipt = "Paid with cash";
        return true;
    }
    std::string name() const override { return "Cash"; }
};

class CreditCardPayment : public PaymentStrategy {
public:
    bool pay(const Order&, std::string& out_receipt) override {
        out_receipt = "Paid with credit card (simulated)";
        return true;
    }
    std::string name() const override { return "CreditCard"; }
};

// Factory helpers
std::unique_ptr<PaymentStrategy> makeCashPayment() {
    return std::make_unique<CashPayment>();
}
std::unique_ptr<PaymentStrategy> makeCreditCardPayment() {
    return std::make_unique<CreditCardPayment>();
}
