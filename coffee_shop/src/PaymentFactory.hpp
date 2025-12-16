#pragma once
#include <memory>
#include <string>
#include "PaymentStrategy.hpp"

class PaymentFactory {
public:
    static std::unique_ptr<PaymentStrategy> Create(
        const std::string& paymentType);
};
