#include "DrinkFactory.hpp"

#include <algorithm>
#include <stdexcept>

#include "Drinks.hpp"

std::unique_ptr<Drink> DrinkFactory::Create(const std::string& drinkType) {
  std::string t = drinkType;
  std::transform(t.begin(), t.end(), t.begin(), ::tolower);
  if (t == "espresso") {
    return std::make_unique<Espresso>();
  } else if (t == "latte") {
    return std::make_unique<Latte>();
  } else if (t == "tea") {
    return std::make_unique<Tea>();
  }
  throw std::invalid_argument("Unknown drink type: " + drinkType);
}

std::vector<std::string> DrinkFactory::ListAvailable() {
  return {"Espresso", "Latte", "Tea"};
}
