##Сборка проекта:
```
cd coffee_shop
mkdir build
cd build
cmake -DBUILD_TESTS=ON .. 
cmake --build .
```

##Запуск тестов:
```
cd build
ctest --output-on-failure
# или напрямую
./coffee-shop-tests
```

##Проверка стиля:
```
bash check_style.sh
```
