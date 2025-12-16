## Сборка проекта:
```
cd coffee_shop
mkdir build
cd build
cmake -DBUILD_TESTS=ON .. 
cmake --build .
```

## Запуск тестов:
```
cd build
ctest --output-on-failure
# или напрямую
cd build
./tests/coffee-shop-tests 
```

## Проверка стиля (из ```pbs/coffee_shop```):
```
bash check_style.sh
```
