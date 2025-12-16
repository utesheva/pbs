#pragma once
#include "Drinks.hpp"

#include <memory>
#include <string>
#include <vector>

class DrinkFactory {
public:
    static std::unique_ptr<Drink> Create(const std::string& drinkType);

    static std::vector<std::string> ListAvailable();
};
