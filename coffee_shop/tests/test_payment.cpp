#include <gtest/gtest.h>
#include <memory>
#include "PaymentFactory.hpp"
#include "PaymentStrategy.hpp"
#include "Order.hpp"
#include "Drinks.hpp"

// Тест создания CashPayment через фабрику
TEST(PaymentFactoryTest, CreateCashPayment) {
    auto payment = PaymentFactory::create("Cash");
    ASSERT_NE(payment, nullptr);
}

// Тест создания CreditCardPayment через фабрику
TEST(PaymentFactoryTest, CreateCreditCardPayment) {
    auto payment = PaymentFactory::create("CreditCard");
    ASSERT_NE(payment, nullptr);
}

// Тест case-insensitive создания способов оплаты
TEST(PaymentFactoryTest, CreateCaseInsensitive) {
    auto payment1 = PaymentFactory::create("cash");
    auto payment2 = PaymentFactory::create("CREDITCARD");
    auto payment3 = PaymentFactory::create("Credit");
    ASSERT_NE(payment1, nullptr);
    ASSERT_NE(payment2, nullptr);
    ASSERT_NE(payment3, nullptr);
}

// Тест на неизвестный способ оплаты
TEST(PaymentFactoryTest, CreateUnknownPayment) {
    EXPECT_THROW(PaymentFactory::create("Bitcoin"), std::invalid_argument);
    EXPECT_THROW(PaymentFactory::create(""), std::invalid_argument);
    EXPECT_THROW(PaymentFactory::create("Unknown"), std::invalid_argument);
}

// Тест оплаты наличными
TEST(PaymentStrategyTest, CashPayment) {
    auto drink = std::make_unique<Espresso>();
    auto payment = PaymentFactory::create("Cash");
    auto order = std::make_shared<Order>(1, std::move(drink), std::move(payment));
    std::string receipt;
    bool result = order->pay(receipt);
    EXPECT_TRUE(result);
    EXPECT_FALSE(receipt.empty());
    EXPECT_EQ(receipt, "Paid with cash");
    EXPECT_EQ(order->status(), Order::Status::Paid);
}

// Тест оплаты кредитной картой
TEST(PaymentStrategyTest, CreditCardPayment) {
    auto drink = std::make_unique<Latte>();
    auto payment = PaymentFactory::create("CreditCard");
    auto order = std::make_shared<Order>(2, std::move(drink), std::move(payment));
    std::string receipt;
    bool result = order->pay(receipt);
    EXPECT_TRUE(result);
    EXPECT_FALSE(receipt.empty());
    EXPECT_EQ(receipt, "Paid with credit card (simulated)");
    EXPECT_EQ(order->status(), Order::Status::Paid);
}

// Тест множественной оплаты (не должна проходить повторно)
TEST(PaymentStrategyTest, DoublePayment) {
    auto drink = std::make_unique<Tea>();
    auto payment = PaymentFactory::create("Cash");
    auto order = std::make_shared<Order>(3, std::move(drink), std::move(payment));
    std::string receipt1, receipt2;
    bool result1 = order->pay(receipt1);
    EXPECT_TRUE(result1);
    EXPECT_EQ(receipt1, "Paid with cash");
    EXPECT_EQ(order->status(), Order::Status::Paid);
}

// Дополнительный тест: проверка содержимого чека
TEST(PaymentStrategyTest, ReceiptContent) {
    auto drink1 = std::make_unique<Espresso>();
    auto payment1 = PaymentFactory::create("Cash");
    auto order1 = std::make_shared<Order>(10, std::move(drink1), std::move(payment1));

    std::string receipt1;
    order1->pay(receipt1);

    EXPECT_NE(receipt1.find("Paid"), std::string::npos);
    EXPECT_NE(receipt1.find("cash"), std::string::npos);

    auto drink2 = std::make_unique<Latte>();
    auto payment2 = PaymentFactory::create("CreditCard");
    auto order2 = std::make_shared<Order>(11, std::move(drink2), std::move(payment2));

    std::string receipt2;
    order2->pay(receipt2);

    EXPECT_NE(receipt2.find("Paid"), std::string::npos);
    EXPECT_NE(receipt2.find("credit card"), std::string::npos);
}
