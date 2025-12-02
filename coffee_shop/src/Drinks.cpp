#include "Drinks.hpp"
#include <string>

std::string Espresso::name() const { return "Espresso"; }
int Espresso::prepareTimeSeconds() const { return 2; }


std::string Latte::name() const { return "Latte"; }
int Latte::prepareTimeSeconds() const { return 4; }

std::string Tea::name() const { return "Tea"; }
int Tea::prepareTimeSeconds() const { return 3; }

