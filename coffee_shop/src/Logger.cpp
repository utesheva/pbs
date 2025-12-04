#include "Logger.hpp"
#include "Order.hpp"
#include <iostream>
#include <chrono>
#include <ctime>

LoggerObserver::LoggerObserver(const std::string& file) {
    ofs_.open(file, std::ios::app);
}

LoggerObserver::~LoggerObserver() {
    if (ofs_.is_open()) ofs_.close();
}

void LoggerObserver::update(const Order& order) {
    std::lock_guard<std::mutex> lk(mtx_);
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::string line = std::string(std::ctime(&t));
    if (!line.empty() && line.back() == '\n') line.pop_back();
    std::string msg = line + " - Order " + std::to_string(order.id())
        + " status: " + order.statusString();
    // to console
    std::cout << msg << std::endl;
    if (ofs_.is_open()) {
        ofs_ << msg << std::endl;
    }
}
