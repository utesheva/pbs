#include <iostream>
#include <memory>
#include <thread>

#include "CoffeeShop.hpp"
#include "HttpServer.hpp"
#include "Logger.hpp"
#include "NotificationDispatcher.hpp"
#include "OrderManager.hpp"

int main() {
    // Start dispatcher early
    auto& disp = NotificationDispatcher::Instance();

    auto shop = std::make_shared<CoffeeShop>();
    shop->Start(3);

    auto logger = std::make_shared<LoggerObserver>("coffee.log");
    auto manager = std::make_shared<OrderManager>(*shop, logger);

    // Pre-bind manager pointer into HttpServer
    HttpServer server(shop, manager);

    // Run server (blocking). Ctrl-C to stop.
    server.run("0.0.0.0", 8080);

    // On exit:
    shop->Stop();
    disp.Stop();
    return 0;
}
