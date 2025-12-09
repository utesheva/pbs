#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Drinks.hpp"

class DrinkFactory {
 public:
  static std::unique_ptr<Drink> create(const std::string& drinkType);

  static std::vector<std::string> listAvailable();
};
