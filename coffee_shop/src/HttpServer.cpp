#include "HttpServer.hpp"
#include "DrinkFactory.hpp"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;

static void make_error_response(httplib::Response& res, int http_code,
                                const std::string& message) {
    res.status = http_code;
    json out = {{"success", false}, {"error", message}, {"code", http_code}};
    res.set_content(out.dump(), "application/json");
}

static void make_ok_response(httplib::Response& res, const json& body) {
    json out = body;
    out["success"] = true;
    res.set_content(out.dump(), "application/json");
}

HttpServer::HttpServer(std::shared_ptr<CoffeeShop> shop,
                       std::shared_ptr<OrderManager> manager)
    : shop_(std::move(shop))
    , manager_(std::move(manager)) {
}

HttpServer::~HttpServer() = default;

void HttpServer::Run(const std::string& host, int port) {
    httplib::Server svr;

    // GET /drinks
    svr.Get("/drinks", [](const httplib::Request&, httplib::Response& res) {
        try {
            json j = json::array();
            for (auto& d : DrinkFactory::ListAvailable()) {
                j.push_back(d);
            }
            make_ok_response(res, json::object({{"drinks", j}}));
        } catch (const std::exception& ex) {
            make_error_response(res, 500,
                                std::string("internal error: ") + ex.what());
        }
    });

    // POST /orders { "drink": "...", "payment": "..." }
    svr.Post("/orders", [this](const httplib::Request& req,
                               httplib::Response& res) {
        try {
            if (req.body.empty()) {
                make_error_response(res, 400, "request body is empty");
                return;
            }
            auto j = json::parse(req.body);
            if (!j.is_object() || !j.contains("drink") || !j.contains("payment")) {
                make_error_response(res, 400,
                                    "missing required fields: 'drink' and 'payment'");
                return;
            }
            if (!j["drink"].is_string() || !j["payment"].is_string()) {
                make_error_response(res, 400,
                                    "fields 'drink' and 'payment' must be strings");
                return;
            }
            std::string drink = j["drink"];
            std::string payment = j["payment"];

            try {
                auto order = manager_->CreateOrder(drink, payment);
                make_ok_response(res, json::object({{"orderId", order->id()}}));
            } catch (const std::invalid_argument& ia) {
                make_error_response(res, 400, ia.what());
            } catch (const std::exception& ex) {
                make_error_response(
                    res, 500, std::string("could not create order: ") + ex.what());
            }
        } catch (const nlohmann::json::parse_error& pe) {
            make_error_response(res, 400, std::string("invalid json: ") + pe.what());
        } catch (const std::exception& ex) {
            make_error_response(res, 500,
                                std::string("internal error: ") + ex.what());
        }
    });

    // GET /orders/{id}
    svr.Get(R"(/orders/(\d+))",
            [this](const httplib::Request& req, httplib::Response& res) {
                try {
                    int id = std::stoi(req.matches[1]);
                    auto o = manager_->GetOrder(id);
                    if (!o) {
                        make_error_response(res, 404, "order not found");
                        return;
                    }
                    json out = {{"orderId", o->Id()},
                                {"drink", o->DrinkName()},
                                {"status", o->StatusString()}};
                    make_ok_response(res, out);
                } catch (const std::exception& ex) {
                    make_error_response(res, 500,
                                        std::string("internal error: ") + ex.what());
                }
            });

    // POST /orders/{id}/pay
    svr.Post(R"(/orders/(\d+)/pay)",
             [this](const httplib::Request& req, httplib::Response& res) {
                 try {
                     int id = std::stoi(req.matches[1]);
                     auto order = manager_->GetOrder(id);
                     if (!order) {
                         make_error_response(res, 404, "order not found");
                         return;
                     }
                     auto [ok, receipt] = manager_->PayOrder(id);
                     if (!ok) {
                         // Payment failed or wasn't possible
                         make_error_response(
                             res, 400, receipt.empty() ? "payment failed" : receipt);
                         return;
                     }
                     make_ok_response(res, json::object({{"receipt", receipt}}));
                 } catch (const std::exception& ex) {
                     make_error_response(res, 500,
                                         std::string("internal error: ") + ex.what());
                 }
             });

    // POST /orders/{id}/cancel
    svr.Post(R"(/orders/(\d+)/cancel)",
             [this](const httplib::Request& req, httplib::Response& res) {
                 try {
                     int id = std::stoi(req.matches[1]);
                     bool ok = shop_->CancelOrder(id);
                     if (!ok) {
                         // Could be already preparing/completed or not found
                         auto o = manager_->GetOrder(id);
                         if (!o) {
                             make_error_response(res, 404, "order not found");
                         } else {
                             make_error_response(res, 400,
                                                 "order cannot be cancelled (it may be "
                                                 "preparing or already completed)");
                         }
                         return;
                     }
                     make_ok_response(
                         res, json::object({{"orderId", id}, {"cancelled", true}}));
                 } catch (const std::exception& ex) {
                     make_error_response(res, 500,
                                         std::string("internal error: ") + ex.what());
                 }
             });

    // Generic not found
    svr.set_exception_handler([](const httplib::Request&, httplib::Response& res,
                                 std::exception& e) {
        make_error_response(res, 500, std::string("server exception: ") + e.what());
    });
    svr.set_error_handler([](const httplib::Request&, httplib::Response& res) {
        // Ensure client gets JSON even for generic errors
        if (res.status == 404) {
            make_error_response(res, 404, "not found");
        } else {
            make_error_response(res, res.status, "http error");
        }
    });

    std::cout << "HTTP server listening on " << host << ":" << port << " ...\n";
    svr.listen(host.c_str(), port);
}
