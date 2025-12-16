#!/bin/bash

# Скрипт для проверки стиля кодирования
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "Checking code style with clang-format..."
echo "Working directory: $SCRIPT_DIR"

# Поиск всех C++ файлов в src и tests
FILES=$(find "$SCRIPT_DIR/src" "$SCRIPT_DIR/tests" -type f \( -name "*.cpp" -o -name "*.hpp" \) 2>/dev/null)

if [ -z "$FILES" ]; then
    echo "No C++ files found in src or tests"
    echo "Trying alternative paths..."
    
    # Если запущен из coffee_shop директории
    FILES=$(find src tests -type f \( -name "*.cpp" -o -name "*.hpp" \) 2>/dev/null)
    
    if [ -z "$FILES" ]; then
        echo "Still no files found.  Please run from project root or coffee_shop directory."
        exit 1
    fi
fi

echo "Found $(echo "$FILES" | wc -l) C++ files"

# Проверка наличия clang-format
if ! command -v clang-format &> /dev/null; then
    echo "clang-format not found.  Please install it:"
    echo "  sudo apt-get install clang-format"
    exit 1
fi

# Проверка стиля
ERRORS=0
for file in $FILES; do
    echo "Checking:  $file"
    clang-format --dry-run --Werror --style=google "$file" 2>&1
    if [ $? -ne 0 ]; then
        echo "Style error in:  $file"
        ERRORS=$((ERRORS + 1))
    fi
done

echo ""
if [ $ERRORS -eq 0 ]; then
    echo "All files pass style check"
    exit 0
else
    echo "Found $ERRORS file(s) with style errors"
    echo "Run 'clang-format -i --style=google <file>' to fix automatically"
    exit 1
fi
