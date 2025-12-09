cmake_minimum_required(VERSION 3.14)
project(coffee-shop VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing

# Fetch third-party header-only libs: nlohmann/json and cpp-httplib
include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG v0.10.6
)
FetchContent_MakeAvailable(httplib)

# Source lists to avoid duplication
set(SRC_COMMON
    src/ClientObserver.cpp
    src/CoffeeShop.cpp
    src/DrinkFactory.cpp
    src/Drinks.cpp
    src/Logger.cpp
    src/NotificationDispatcher.cpp
    src/Observer.hpp
    src/Order.cpp
    src/OrderManager.cpp
    src/PaymentStrategy.cpp
    src/PaymentFactory.cpp
)

# Main demo executable (CLI)
add_executable(coffee-shop
    src/main.cpp
    ${SRC_COMMON}
)
target_include_directories(coffee-shop PRIVATE src)

# HTTP server executable
add_executable(coffee-server
    src/server_main.cpp
    src/HttpServer.cpp
    ${SRC_COMMON}
)
target_include_directories(coffee-server PRIVATE src ${httplib_SOURCE_DIR} ${nlohmann_json_SOURCE_DIR}/include)

# Client application (CLI) to talk to the HTTP server
add_executable(client-app
    src/ClientObserver.cpp
    src/ClientApp.cpp
)
target_include_directories(client-app PRIVATE src ${httplib_SOURCE_DIR} ${nlohmann_json_SOURCE_DIR}/include)

