#include "Drinks.hpp"

#include <string>

std::string Espresso::Name() const { return "Espresso"; }
int Espresso::PrepareTimeSeconds() const { return 10; }

std::string Latte::Name() const { return "Latte"; }
int Latte::PrepareTimeSeconds() const { return 20; }

std::string Tea::Name() const { return "Tea"; }
int Tea::PrepareTimeSeconds() const { return 15; }
