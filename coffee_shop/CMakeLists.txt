cmake_minimum_required(VERSION 3.14)
project(coffee-shop VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy checks" OFF)

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

if(BUILD_TESTS)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  enable_testing()
  include(GoogleTest)
endif()

set(SRC_COMMON
    src/DrinkFactory.cpp
    src/Drinks.cpp
    src/PaymentStrategy.cpp
    src/PaymentFactory.cpp
    src/Order.cpp
    src/OrderManager.cpp
    src/CoffeeShop.cpp
    src/Observer.cpp
    src/Logger.cpp
    src/NotificationDispatcher.cpp
    src/ClientObserver.cpp
)

add_library(coffee-shop-core STATIC ${SRC_COMMON})
target_include_directories(coffee-shop-core PUBLIC src)

add_executable(coffee-shop
    src/main.cpp
)
target_include_directories(coffee-shop PRIVATE src)
target_link_libraries(coffee-shop PRIVATE coffee-shop-core)

add_executable(coffee-server
    src/server_main.cpp
    src/HttpServer.cpp
)
target_include_directories(coffee-server PRIVATE src ${httplib_SOURCE_DIR} ${nlohmann_json_SOURCE_DIR}/include)
target_link_libraries(coffee-server PRIVATE coffee-shop-core)

add_executable(client-app
    src/ClientApp.cpp
)
target_include_directories(client-app PRIVATE src ${httplib_SOURCE_DIR} ${nlohmann_json_SOURCE_DIR}/include)
target_link_libraries(client-app PRIVATE coffee-shop-core)

if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
  if(CLANG_TIDY_EXE)
    set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,readability-*,modernize-*,performance-*,bugprone-*")
    set_target_properties(coffee-shop-core PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    message(STATUS "clang-tidy enabled")
  else()
    message(WARNING "clang-tidy not found")
  endif()
endif()

# Тесты
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
