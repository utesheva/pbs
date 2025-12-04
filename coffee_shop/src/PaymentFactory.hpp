#pragma once
#include <memory>
#include <string>
#include "PaymentStrategy.hpp"

class PaymentFactory {
public:
    // Create payment strategy by name (lower/upper-case insensitive).
    // Throws std::invalid_argument if unknown.
    static std::unique_ptr<PaymentStrategy> create(const std::string& paymentType);
};
