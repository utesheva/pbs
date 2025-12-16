#pragma once
#include <string>

class Drink {
 public:
  virtual ~Drink() = default;
  virtual std::string Name() const = 0;
  virtual int PrepareTimeSeconds() const = 0;
};

class Espresso : public Drink {
 public:
  std::string Name() const override;
  int PrepareTimeSeconds() const override;
};

class Latte : public Drink {
 public:
  std::string Name() const override;
  int PrepareTimeSeconds() const override;
};

class Tea : public Drink {
 public:
  std::string Name() const override;
  int PrepareTimeSeconds() const override;
};
