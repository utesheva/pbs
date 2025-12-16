#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <thread>

#include "Drinks.hpp"
#include "Observer.hpp"
#include "Order.hpp"
#include "PaymentFactory.hpp"

// Mock Observer для тестирования паттерна Observer
class MockObserver : public IObserver {
 public:
  MOCK_METHOD(void, Update, (const Order& order), (override));
};

// Тест создания заказа
TEST(OrderTest, CreateOrder) {
  auto drink = std::make_unique<Espresso>();
  auto payment = PaymentFactory::Create("Cash");
  auto order = std::make_shared<Order>(1, std::move(drink), std::move(payment));

  EXPECT_EQ(order->Id(), 1);
  EXPECT_EQ(order->DrinkName(), "Espresso");
  EXPECT_EQ(order->PrepareTime(), 10);
  EXPECT_EQ(order->Status(), Order::StatusType::Placed);
}

// Тест смены статуса заказа
TEST(OrderTest, ChangeStatus) {
  auto drink = std::make_unique<Latte>();
  auto payment = PaymentFactory::Create("CreditCard");
  auto order = std::make_shared<Order>(2, std::move(drink), std::move(payment));

  EXPECT_EQ(order->Status(), Order::StatusType::Placed);

  order->SetStatus(Order::StatusType::Preparing);
  EXPECT_EQ(order->Status(), Order::StatusType::Preparing);

  order->SetStatus(Order::StatusType::Ready);
  EXPECT_EQ(order->Status(), Order::StatusType::Ready);

  order->SetStatus(Order::StatusType::Completed);
  EXPECT_EQ(order->Status(), Order::StatusType::Completed);
}

// Тест строкового представления статуса
TEST(OrderTest, StatusString) {
  auto drink = std::make_unique<Tea>();
  auto payment = PaymentFactory::Create("Cash");
  auto order = std::make_shared<Order>(3, std::move(drink), std::move(payment));

  EXPECT_EQ(order->StatusString(), "Placed");

  order->SetStatus(Order::StatusType::Preparing);
  EXPECT_EQ(order->StatusString(), "Preparing");

  order->SetStatus(Order::StatusType::Ready);
  EXPECT_EQ(order->StatusString(), "Ready");

  order->SetStatus(Order::StatusType::Completed);
  EXPECT_EQ(order->StatusString(), "Completed");

  order->SetStatus(Order::StatusType::Cancelled);
  EXPECT_EQ(order->StatusString(), "Cancelled");
}

// Тест паттерна Observer - уведомление наблюдателей
TEST(OrderTest, ObserverNotification) {
  auto drink = std::make_unique<Espresso>();
  auto payment = PaymentFactory::Create("Cash");
  auto order = std::make_shared<Order>(4, std::move(drink), std::move(payment));

  auto mockObserver = std::make_shared<MockObserver>();

  // Ожидаем, что Update будет вызван при изменении статуса
  EXPECT_CALL(*mockObserver, Update(::testing::_)).Times(::testing::AtLeast(1));

  order->Attach(mockObserver);
  order->SetStatus(Order::StatusType::Preparing);

  // Даем время диспетчеру обработать уведомление
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// Тест отписки наблюдателя
TEST(OrderTest, ObserverDetach) {
  auto drink = std::make_unique<Latte>();
  auto payment = PaymentFactory::Create("CreditCard");
  auto order = std::make_shared<Order>(5, std::move(drink), std::move(payment));

  auto mockObserver = std::make_shared<MockObserver>();

  order->Attach(mockObserver);
  order->Detach(mockObserver);

  // После отписки Update не должен вызываться
  EXPECT_CALL(*mockObserver, Update(::testing::_)).Times(0);

  order->SetStatus(Order::StatusType::Preparing);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// Тест потокобезопасности статуса
TEST(OrderTest, ThreadSafeStatus) {
  auto drink = std::make_unique<Tea>();
  auto payment = PaymentFactory::Create("Cash");
  auto order = std::make_shared<Order>(6, std::move(drink), std::move(payment));

  // Запускаем несколько потоков, изменяющих статус
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) {
    threads.emplace_back([order]() {
      order->SetStatus(Order::StatusType::Preparing);
      order->Status();  // читаем статус
      order->SetStatus(Order::StatusType::Ready);
    });
  }

  for (auto& t : threads) {
    t.join();
  }

  // Проверяем, что заказ не в невалидном состоянии
  auto finalStatus = order->Status();
  EXPECT_TRUE(finalStatus == Order::StatusType::Preparing ||
              finalStatus == Order::StatusType::Ready);
}
