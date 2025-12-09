#include "Drinks.hpp"

#include <string>

std::string Espresso::name() const { return "Espresso"; }
int Espresso::prepareTimeSeconds() const { return 10; }

std::string Latte::name() const { return "Latte"; }
int Latte::prepareTimeSeconds() const { return 20; }

std::string Tea::name() const { return "Tea"; }
int Tea::prepareTimeSeconds() const { return 15; }
