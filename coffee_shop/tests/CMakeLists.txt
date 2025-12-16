#Тесты для бизнес - логики
add_executable(coffee - shop -
               tests test_drinks.cpp test_payment.cpp test_order
                   .cpp test_factories.cpp test_multithreading.cpp)

    target_link_libraries(coffee - shop - tests PRIVATE coffee - shop -
                          core GTest::gtest GTest::gtest_main GTest::gmock)

        target_include_directories(coffee - shop - tests PRIVATE../ src)

#Добавляем тесты в CTest
            gtest_discover_tests(coffee - shop - tests)

#Дополнительный исполняемый файл для проверки стиля
                add_test(NAME StyleCheck COMMAND ${CMAKE_COMMAND} - E echo
                         "Style check placeholder - integrate with "
                         "clang-format or cpplint")
