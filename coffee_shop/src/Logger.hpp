#pragma once
#include "Observer.hpp"
#include <mutex>
#include <fstream>
#include <string>

class LoggerObserver : public IObserver {
public:
    LoggerObserver(const std::string& file = "coffee.log");
    ~LoggerObserver();
    void update(const Order& order) override;
private:
    std::mutex mtx_;
    std::ofstream ofs_;
};
