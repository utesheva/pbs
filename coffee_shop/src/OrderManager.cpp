#include "OrderManager.hpp"

#include "DrinkFactory.hpp"
#include "Logger.hpp"
#include "PaymentFactory.hpp"

OrderManager::OrderManager(CoffeeShop& shop,
                           std::shared_ptr<LoggerObserver> logger)
    : shop_(shop), logger_(logger) {}

OrderManager::~OrderManager() = default;

std::shared_ptr<Order> OrderManager::CreateOrder(
    const std::string& drinkType, const std::string& paymentType) {
  int id = nextId_.fetch_add(1);
  auto drink = DrinkFactory::Create(drinkType);
  auto payment = PaymentFactory::Create(paymentType);

  auto order =
      std::make_shared<Order>(id, std::move(drink), std::move(payment));

  if (logger_) {
    order->Attach(logger_);
  }

  {
    std::lock_guard<std::mutex> lk(mtx_);
    orders_[id] = order;
  }

  order->SetStatus(Order::StatusType::Placed);

  shop_.SubmitOrder(order);
  return order;
}

std::shared_ptr<Order> OrderManager::GetOrder(int id) {
  std::lock_guard<std::mutex> lk(mtx_);
  auto it = orders_.find(id);
  if (it != orders_.end()) {
    return it->second;
  }
  return nullptr;
}

std::pair<bool, std::string> OrderManager::PayOrder(int id) {
  auto o = GetOrder(id);
  if (!o) {
    return {false, "Order not found"};
  }
  std::string receipt;
  bool ok = o->Pay(receipt);
  return {ok, receipt};
}
