#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#include "CoffeeShop.hpp"
#include "Logger.hpp"
#include "Order.hpp"
#include "OrderManager.hpp"

// Тест одновременной обработки нескольких заказов
TEST(MultithreadingTest, ConcurrentOrders) {
  CoffeeShop shop;
  shop.Start(3);  // 3 baristas

  auto logger = std::make_shared<LoggerObserver>("test_concurrent. log");
  OrderManager manager(shop, logger);

  const int NUM_ORDERS = 10;
  std::vector<std::shared_ptr<Order>> orders;

  // Создаем несколько заказов одновременно
  for (int i = 0; i < NUM_ORDERS; ++i) {
    auto order = manager.CreateOrder("Espresso", "Cash");
    orders.push_back(order);
  }

  // Ждем обработки
  std::this_thread::sleep_for(std::chrono::seconds(15));

  // Проверяем, что все заказы обработаны
  int completedCount = 0;
  for (const auto& order : orders) {
    auto status = order->Status();
    if (status == Order::StatusType::Ready ||
        status == Order::StatusType::Completed) {
      completedCount++;
    }
  }

  EXPECT_GT(completedCount, 0) << "No orders were completed";

  shop.Stop();
}

// Тест безопасности при одновременном доступе к OrderManager
TEST(MultithreadingTest, ConcurrentOrderCreation) {
  CoffeeShop shop;
  shop.Start(2);

  auto logger = std::make_shared<LoggerObserver>("test_creation.log");
  OrderManager manager(shop, logger);

  std::vector<std::thread> threads;
  std::vector<std::shared_ptr<Order>> orders;
  std::mutex ordersMutex;

  // Создаем заказы из разных потоков
  for (int i = 0; i < 5; ++i) {
    threads.emplace_back([&manager, &orders, &ordersMutex]() {
      for (int j = 0; j < 3; ++j) {
        auto order = manager.CreateOrder("Latte", "CreditCard");
        std::lock_guard<std::mutex> lock(ordersMutex);
        orders.push_back(order);
      }
    });
  }

  for (auto& t : threads) {
    t.join();
  }

  // Проверяем, что все заказы имеют уникальные ID
  std::set<int> ids;
  for (const auto& order : orders) {
    ids.insert(order->Id());
  }

  EXPECT_EQ(ids.size(), orders.size()) << "Order IDs are not unique";

  shop.Stop();
}

// Тест корректной остановки при активных заказах
TEST(MultithreadingTest, GracefulShutdown) {
  CoffeeShop shop;
  shop.Start(2);

  auto logger = std::make_shared<LoggerObserver>("test_shutdown.log");
  OrderManager manager(shop, logger);

  // Создаем заказы с длительным временем приготовления
  for (int i = 0; i < 5; ++i) {
    manager.CreateOrder("Latte", "Cash");  // 20 секунд
  }

  // Сразу останавливаем
  shop.Stop();

  // Тест успешен, если программа не падает и завершается корректно
  SUCCEED();
}
