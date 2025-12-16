#include <httplib.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

static void print_usage() {
  std::cout << "Usage:\n"
            << "  client-app list\n"
            << "  client-app order <drink> <payment>\n"
            << "  client-app status <orderId>\n"
            << "  client-app pay <orderId>\n"
            << "  client-app cancel <orderId>\n"
            << "Options (env): COFFEE_HOST, COFFEE_PORT\n"
            << "Payment options: cash, cashpayment, credit, creditcard, "
               "creditcardpayment\n";
}

static std::string host_from_env() {
  const char* h = std::getenv("COFFEE_HOST");
  return h ? std::string(h) : "127.0.0.1";
}
static int port_from_env() {
  const char* p = std::getenv("COFFEE_PORT");
  return p ? std::atoi(p) : 8080;
}

static bool print_error_response(const httplib::Response& res) {
  try {
    auto j = json::parse(res.body);
    if (j.contains("error")) {
      std::cerr << "Error: " << j["error"].get<std::string>() << " (code "
                << j.value("code", res.status) << ")\n";
      return true;
    }
  } catch (...) {
    // ignore parse errors
  }
  std::cerr << "HTTP error: " << res.status << "\n";
  return true;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage();
    return 1;
  }
  std::string host = host_from_env();
  int port = port_from_env();
  httplib::Client cli(host.c_str(), port);
  cli.set_read_timeout(5, 0);  // 5 seconds

  std::string cmd = argv[1];
  try {
    if (cmd == "list") {
      auto res = cli.Get("/drinks");
      if (!res) {
        std::cerr << "Failed to connect to server\n";
        return 2;
      }
      if (res->status != 200) {
        print_error_response(*res);
        return 3;
      }
      auto j = json::parse(res->body);
      if (j.contains("drinks")) {
        std::cout << "Available drinks:\n";
        for (auto& d : j["drinks"]) {
          std::cout << " - " << d.get<std::string>() << "\n";
        }
        return 0;
      }
      std::cout << res->body << "\n";
      return 0;
    } else if (cmd == "order") {
      if (argc < 4) {
        print_usage();
        return 1;
      }
      json req = {{"drink", argv[2]}, {"payment", argv[3]}};
      auto res = cli.Post("/orders", req.dump(), "application/json");
      if (!res) {
        std::cerr << "Failed to connect to server\n";
        return 2;
      }
      if (res->status != 200) {
        print_error_response(*res);
        return 3;
      }
      auto j = json::parse(res->body);
      std::cout << "Order created, id=" << j["orderId"].get<int>() << "\n";
      return 0;
    } else if (cmd == "status") {
      if (argc < 3) {
        print_usage();
        return 1;
      }
      std::string path = "/orders/" + std::string(argv[2]);
      auto res = cli.Get(path.c_str());
      if (!res) {
        std::cerr << "Failed to connect to server\n";
        return 2;
      }
      if (res->status != 200) {
        print_error_response(*res);
        return 3;
      }
      auto j = json::parse(res->body);
      std::cout << "Order " << j["orderId"].get<int>() << " - "
                << j["status"].get<std::string>() << "\n";
      return 0;
    } else if (cmd == "pay") {
      if (argc < 3) {
        print_usage();
        return 1;
      }
      std::string path = "/orders/" + std::string(argv[2]) + "/pay";
      auto res = cli.Post(path.c_str(), "", "application/json");
      if (!res) {
        std::cerr << "Failed to connect to server\n";
        return 2;
      }
      if (res->status != 200) {
        print_error_response(*res);
        return 3;
      }
      auto j = json::parse(res->body);
      std::cout << "Payment successful. Receipt: "
                << j["receipt"].get<std::string>() << "\n";
      return 0;
    } else if (cmd == "cancel") {
      if (argc < 3) {
        print_usage();
        return 1;
      }
      std::string path = "/orders/" + std::string(argv[2]) + "/cancel";
      auto res = cli.Post(path.c_str(), "", "application/json");
      if (!res) {
        std::cerr << "Failed to connect to server\n";
        return 2;
      }
      if (res->status != 200) {
        print_error_response(*res);
        return 3;
      }
      auto j = json::parse(res->body);
      std::cout << "Order " << j["orderId"].get<int>()
                << " cancelled: " << (j["cancelled"].get<bool>() ? "yes" : "no")
                << "\n";
      return 0;
    } else {
      print_usage();
      return 1;
    }
  } catch (const std::exception& ex) {
    std::cerr << "Client exception: " << ex.what() << "\n";
    return 4;
  }
}
