#include "PaymentFactory.hpp"
#include <algorithm>
#include <stdexcept>
#include "PaymentStrategy.hpp"

std::unique_ptr<PaymentStrategy> PaymentFactory::create(
    const std::string& paymentType) {
  std::string p = paymentType;
  std::transform(p.begin(), p.end(), p.begin(), ::tolower);
  if (p == "cash" || p == "cashpayment") {
    return makeCashPayment();
  } else if (p == "credit" || p == "creditcard" || p == "creditcardpayment") {
    return makeCreditCardPayment();
  }
  throw std::invalid_argument("Unknown payment type: " + paymentType);
}
