#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <thread>
#include <chrono>
#include "Order.hpp"
#include "Drinks.hpp"
#include "PaymentFactory.hpp"
#include "Observer.hpp"

// Mock Observer для тестирования паттерна Observer
class MockObserver: public IObserver {
public:
    MOCK_METHOD(void, update, (const Order& order), (override));
};

// Тест создания заказа
TEST(OrderTest, CreateOrder) {
    auto drink = std::make_unique<Espresso>();
    auto payment = PaymentFactory::create("Cash");
    auto order = std::make_shared<Order>(1, std::move(drink), std::move(payment));

    EXPECT_EQ(order->id(), 1);
    EXPECT_EQ(order->drinkName(), "Espresso");
    EXPECT_EQ(order->prepareTime(), 10);
    EXPECT_EQ(order->status(), Order::Status::Placed);
}

// Тест смены статуса заказа
TEST(OrderTest, ChangeStatus) {
    auto drink = std::make_unique<Latte>();
    auto payment = PaymentFactory::create("CreditCard");
    auto order = std::make_shared<Order>(2, std::move(drink), std::move(payment));

    EXPECT_EQ(order->status(), Order::Status::Placed);

    order->setStatus(Order::Status::Preparing);
    EXPECT_EQ(order->status(), Order::Status::Preparing);

    order->setStatus(Order::Status::Ready);
    EXPECT_EQ(order->status(), Order::Status::Ready);

    order->setStatus(Order::Status::Completed);
    EXPECT_EQ(order->status(), Order::Status::Completed);
}

// Тест строкового представления статуса
TEST(OrderTest, StatusString) {
    auto drink = std::make_unique<Tea>();
    auto payment = PaymentFactory::create("Cash");
    auto order = std::make_shared<Order>(3, std::move(drink), std::move(payment));

    EXPECT_EQ(order->statusString(), "Placed");

    order->setStatus(Order::Status::Preparing);
    EXPECT_EQ(order->statusString(), "Preparing");

    order->setStatus(Order::Status::Ready);
    EXPECT_EQ(order->statusString(), "Ready");

    order->setStatus(Order::Status::Completed);
    EXPECT_EQ(order->statusString(), "Completed");

    order->setStatus(Order::Status::Cancelled);
    EXPECT_EQ(order->statusString(), "Cancelled");
}

// Тест паттерна Observer - уведомление наблюдателей
TEST(OrderTest, ObserverNotification) {
    auto drink = std::make_unique<Espresso>();
    auto payment = PaymentFactory::create("Cash");
    auto order = std::make_shared<Order>(4, std::move(drink), std::move(payment));

    auto mockObserver = std::make_shared<MockObserver>();

    // Ожидаем, что update будет вызван при изменении статуса
    EXPECT_CALL(*mockObserver, update(::testing::_))
        .Times(::testing::AtLeast(1));

    order->attach(mockObserver);
    order->setStatus(Order::Status::Preparing);

    // Даем время диспетчеру обработать уведомление
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// Тест отписки наблюдателя
TEST(OrderTest, ObserverDetach) {
    auto drink = std::make_unique<Latte>();
    auto payment = PaymentFactory::create("CreditCard");
    auto order = std::make_shared<Order>(5, std::move(drink), std::move(payment));

    auto mockObserver = std::make_shared<MockObserver>();

    order->attach(mockObserver);
    order->detach(mockObserver);

    // После отписки update не должен вызываться
    EXPECT_CALL(*mockObserver, update(::testing::_))
        .Times(0);

    order->setStatus(Order::Status::Preparing);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// Тест потокобезопасности статуса
TEST(OrderTest, ThreadSafeStatus) {
    auto drink = std::make_unique<Tea>();
    auto payment = PaymentFactory::create("Cash");
    auto order = std::make_shared<Order>(6, std::move(drink), std::move(payment));

    // Запускаем несколько потоков, изменяющих статус
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([order]() {
            order->setStatus(Order::Status::Preparing);
            order->status(); // читаем статус
            order->setStatus(Order::Status::Ready);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Проверяем, что заказ не в невалидном состоянии
    auto finalStatus = order->status();
    EXPECT_TRUE(finalStatus == Order::Status::Preparing ||
                finalStatus == Order::Status::Ready);
}
