#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Drinks.hpp"

class DrinkFactory {
 public:
  static std::unique_ptr<Drink> Create(const std::string& drinkType);

  static std::vector<std::string> ListAvailable();
};
