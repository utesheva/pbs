#pragma once
#include "Observer.hpp"

#include <fstream>
#include <mutex>
#include <string>

class LoggerObserver: public IObserver {
public:
    LoggerObserver(const std::string& file = "coffee.log");
    ~LoggerObserver();
    void Update(const Order& order) override;

private:
    std::mutex mtx_;
    std::ofstream ofs_;
};
