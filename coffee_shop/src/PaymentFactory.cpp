#include "PaymentFactory.hpp"
#include <algorithm>
#include <stdexcept>
#include "PaymentStrategy.hpp"

std::unique_ptr<PaymentStrategy> PaymentFactory::Create(
    const std::string& paymentType) {
    std::string p = paymentType;
    std::transform(p.begin(), p.end(), p.begin(), ::tolower);
    if (p == "cash" || p == "cashpayment") {
        return MakeCashPayment();
    } else if (p == "credit" || p == "creditcard" || p == "creditcardpayment") {
        return MakeCreditCardPayment();
    }
    throw std::invalid_argument("Unknown payment type: " + paymentType);
}
