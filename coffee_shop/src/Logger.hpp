#pragma once
#include <fstream>
#include <mutex>
#include <string>

#include "Observer.hpp"

class LoggerObserver: public IObserver {
public:
    LoggerObserver(const std::string& file = "coffee.log");
    ~LoggerObserver();
    void update(const Order& order) override;

private:
    std::mutex mtx_;
    std::ofstream ofs_;
};
