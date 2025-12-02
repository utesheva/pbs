#pragma once
#include "Drinks.hpp"
#include <memory>
#include <string>
#include <vector>

class DrinkFactory {
public:
    // Throws std::invalid_argument if unknown drink
    static std::unique_ptr<Drink> create(const std::string& drinkType);
    
    static std::vector<std::string> listAvailable();
};
