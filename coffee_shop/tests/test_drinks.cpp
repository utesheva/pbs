#include <gtest/gtest.h>
#include <memory>
#include "DrinkFactory.hpp"
#include "Drinks.hpp"

// Тест создания Espresso через фабрику
TEST(DrinkFactoryTest, CreateEspresso) {
    auto drink = DrinkFactory::create("Espresso");
    ASSERT_NE(drink, nullptr);
    EXPECT_EQ(drink->name(), "Espresso");
    EXPECT_EQ(drink->prepareTimeSeconds(), 10);
}

// Тест создания Latte через фабрику
TEST(DrinkFactoryTest, CreateLatte) {
    auto drink = DrinkFactory::create("Latte");
    ASSERT_NE(drink, nullptr);
    EXPECT_EQ(drink->name(), "Latte");
    EXPECT_EQ(drink->prepareTimeSeconds(), 20);
}

// Тест создания Tea через фабрику
TEST(DrinkFactoryTest, CreateTea) {
    auto drink = DrinkFactory::create("Tea");
    ASSERT_NE(drink, nullptr);
    EXPECT_EQ(drink->name(), "Tea");
    EXPECT_EQ(drink->prepareTimeSeconds(), 15);
}

// Тест case-insensitive создания напитков
TEST(DrinkFactoryTest, CreateCaseInsensitive) {
    auto drink1 = DrinkFactory::create("espresso");
    auto drink2 = DrinkFactory::create("LATTE");
    auto drink3 = DrinkFactory::create("tEa");

    ASSERT_NE(drink1, nullptr);
    ASSERT_NE(drink2, nullptr);
    ASSERT_NE(drink3, nullptr);

    EXPECT_EQ(drink1->name(), "Espresso");
    EXPECT_EQ(drink2->name(), "Latte");
    EXPECT_EQ(drink3->name(), "Tea");
}

// Тест на неизвестный тип напитка
TEST(DrinkFactoryTest, CreateUnknownDrink) {
    EXPECT_THROW(DrinkFactory::create("Cappuccino"), std::invalid_argument);
    EXPECT_THROW(DrinkFactory::create(""), std::invalid_argument);
    EXPECT_THROW(DrinkFactory::create("Unknown"), std::invalid_argument);
}

// Тест списка доступных напитков
TEST(DrinkFactoryTest, ListAvailable) {
    auto drinks = DrinkFactory::listAvailable();
    EXPECT_EQ(drinks.size(), 3);
    EXPECT_NE(std::find(drinks.begin(), drinks.end(), "Espresso"), drinks.end());
    EXPECT_NE(std::find(drinks.begin(), drinks.end(), "Latte"), drinks.end());
    EXPECT_NE(std::find(drinks.begin(), drinks.end(), "Tea"), drinks.end());
}

// Тест конкретных классов напитков
TEST(DrinksTest, EspressoProperties) {
    Espresso espresso;
    EXPECT_EQ(espresso.name(), "Espresso");
    EXPECT_EQ(espresso.prepareTimeSeconds(), 10);
}

TEST(DrinksTest, LatteProperties) {
    Latte latte;
    EXPECT_EQ(latte.name(), "Latte");
    EXPECT_EQ(latte.prepareTimeSeconds(), 20);
}

TEST(DrinksTest, TeaProperties) {
    Tea tea;
    EXPECT_EQ(tea.name(), "Tea");
    EXPECT_EQ(tea.prepareTimeSeconds(), 15);
}
