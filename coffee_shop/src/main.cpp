#include <iostream>
#include <memory>
#include <thread>

#include "ClientObserver.hpp"
#include "CoffeeShop.hpp"
#include "DrinkFactory.hpp"
#include "Logger.hpp"
#include "Order.hpp"
#include "OrderManager.hpp"
#include "PaymentStrategy.hpp"

int main() {
    CoffeeShop shop;
    shop.start(3); // 3 baristas

    auto logger = std::make_shared<LoggerObserver>("coffee.log");
    OrderManager manager(shop, logger);

    std::cout << "Available drinks:\n";
    for (auto& d : DrinkFactory::listAvailable()) {
        std::cout << " - " << d << "\n";
    }

    // Симуляция клиента: создаём заказ через OrderManager, подписываемся
    // клиентским observer'ом
    auto clientObs = std::make_shared<ClientObserver>(
        0); // orderId можно обновить при получении, для simplicity ignore
    auto order = manager.createOrder("Espresso", "CreditCard");
    order->attach(clientObs);

    std::cout << "Created order id=" << order->id()
              << " drink=" << order->drinkName() << "\n";

    // Подождём, пока статус изменится (например, до Completed)
    bool notified = clientObs->waitForStatusChange(10000); // timeout 10s
    if (notified) {
        std::cout << "Client observer noticed status: " << clientObs->lastStatus()
                  << "\n";
    } else {
        std::cout << "Timeout waiting for status change\n";
    }

    // Оплатим (демонстрация)
    {
        auto [ok, receipt] = manager.payOrder(order->id());
        std::cout << "Payment result: " << ok << ", receipt: " << receipt << "\n";
    }

    // Дождёмся обработки (несколько секунд)
    std::this_thread::sleep_for(std::chrono::seconds(6));

    shop.stop();
    return 0;
}
