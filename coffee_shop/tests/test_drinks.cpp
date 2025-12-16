#include <gtest/gtest.h>

#include <memory>

#include "DrinkFactory.hpp"
#include "Drinks.hpp"

// Тест создания Espresso через фабрику
TEST(DrinkFactoryTest, CreateEspresso) {
  auto drink = DrinkFactory::Create("Espresso");
  ASSERT_NE(drink, nullptr);
  EXPECT_EQ(drink->Name(), "Espresso");
  EXPECT_EQ(drink->PrepareTimeSeconds(), 10);
}

// Тест создания Latte через фабрику
TEST(DrinkFactoryTest, CreateLatte) {
  auto drink = DrinkFactory::Create("Latte");
  ASSERT_NE(drink, nullptr);
  EXPECT_EQ(drink->Name(), "Latte");
  EXPECT_EQ(drink->PrepareTimeSeconds(), 20);
}

// Тест создания Tea через фабрику
TEST(DrinkFactoryTest, CreateTea) {
  auto drink = DrinkFactory::Create("Tea");
  ASSERT_NE(drink, nullptr);
  EXPECT_EQ(drink->Name(), "Tea");
  EXPECT_EQ(drink->PrepareTimeSeconds(), 15);
}

// Тест case-insensitive создания напитков
TEST(DrinkFactoryTest, CreateCaseInsensitive) {
  auto drink1 = DrinkFactory::Create("espresso");
  auto drink2 = DrinkFactory::Create("LATTE");
  auto drink3 = DrinkFactory::Create("tEa");

  ASSERT_NE(drink1, nullptr);
  ASSERT_NE(drink2, nullptr);
  ASSERT_NE(drink3, nullptr);

  EXPECT_EQ(drink1->Name(), "Espresso");
  EXPECT_EQ(drink2->Name(), "Latte");
  EXPECT_EQ(drink3->Name(), "Tea");
}

// Тест на неизвестный тип напитка
TEST(DrinkFactoryTest, CreateUnknownDrink) {
  EXPECT_THROW(DrinkFactory::Create("Cappuccino"), std::invalid_argument);
  EXPECT_THROW(DrinkFactory::Create(""), std::invalid_argument);
  EXPECT_THROW(DrinkFactory::Create("Unknown"), std::invalid_argument);
}

// Тест списка доступных напитков
TEST(DrinkFactoryTest, ListAvailable) {
  auto drinks = DrinkFactory::ListAvailable();
  EXPECT_EQ(drinks.size(), 3);
  EXPECT_NE(std::find(drinks.begin(), drinks.end(), "Espresso"), drinks.end());
  EXPECT_NE(std::find(drinks.begin(), drinks.end(), "Latte"), drinks.end());
  EXPECT_NE(std::find(drinks.begin(), drinks.end(), "Tea"), drinks.end());
}

// Тест конкретных классов напитков
TEST(DrinksTest, EspressoProperties) {
  Espresso espresso;
  EXPECT_EQ(espresso.Name(), "Espresso");
  EXPECT_EQ(espresso.PrepareTimeSeconds(), 10);
}

TEST(DrinksTest, LatteProperties) {
  Latte latte;
  EXPECT_EQ(latte.Name(), "Latte");
  EXPECT_EQ(latte.PrepareTimeSeconds(), 20);
}

TEST(DrinksTest, TeaProperties) {
  Tea tea;
  EXPECT_EQ(tea.Name(), "Tea");
  EXPECT_EQ(tea.PrepareTimeSeconds(), 15);
}
