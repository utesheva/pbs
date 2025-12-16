#include <gtest/gtest.h>
#include "DrinkFactory.hpp"
#include "PaymentFactory.hpp"

// Интеграционный тест:  создание напитков и способов оплаты
TEST(FactoriesIntegrationTest, CreateAllCombinations) {
    auto drinks = DrinkFactory::listAvailable();
    std::vector<std::string> payments = {"Cash", "CreditCard"};

    for (const auto& drinkName : drinks) {
        for (const auto& paymentName : payments) {
            auto drink = DrinkFactory::create(drinkName);
            auto payment = PaymentFactory::create(paymentName);

            ASSERT_NE(drink, nullptr) << "Failed to create " << drinkName;
            ASSERT_NE(payment, nullptr) << "Failed to create " << paymentName;

            EXPECT_EQ(drink->name(), drinkName);
        }
    }
}

// Тест граничных случаев
TEST(FactoriesEdgeCasesTest, EmptyStrings) {
    EXPECT_THROW(DrinkFactory::create(""), std::invalid_argument);
    EXPECT_THROW(PaymentFactory::create(""), std::invalid_argument);
}

TEST(FactoriesEdgeCasesTest, WhitespaceStrings) {
    EXPECT_THROW(DrinkFactory::create("   "), std::invalid_argument);
    EXPECT_THROW(PaymentFactory::create("   "), std::invalid_argument);
}

TEST(FactoriesEdgeCasesTest, SpecialCharacters) {
    EXPECT_THROW(DrinkFactory::create("Espresso! @#"), std::invalid_argument);
    EXPECT_THROW(PaymentFactory::create("Cash$$$"), std::invalid_argument);
}
