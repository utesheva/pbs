#pragma once
#include <memory>
#include "CoffeeShop.hpp"
#include "Logger.hpp"
#include "OrderManager.hpp"

class HttpServer {
public:
    HttpServer(std::shared_ptr<CoffeeShop> shop,
               std::shared_ptr<OrderManager> manager);
    ~HttpServer();

    void run(const std::string& host = "0.0.0.0", int port = 8080);

private:
    std::shared_ptr<CoffeeShop> shop_;
    std::shared_ptr<OrderManager> manager_;
    std::shared_ptr<LoggerObserver> logger_;
};
