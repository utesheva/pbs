#pragma once
#include <string>

class Drink {
public:
    virtual ~Drink() = default;
    virtual std::string name() const = 0;
    virtual int prepareTimeSeconds() const = 0;
};

class Espresso : public Drink {
public:
    std::string name() const override;
    int prepareTimeSeconds() const override;
};

class Latte : public Drink {
public:
    std::string name() const override;
    int prepareTimeSeconds() const override;
};

class Tea : public Drink {
public:
    std::string name() const override;
    int prepareTimeSeconds() const override;
};
