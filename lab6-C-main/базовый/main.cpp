#include "Recursion.h"
#include <iostream>

int main() {
    setupConsole();
    const std::vector<int> values{1, 3, 5, 7, 9};
    // Находим позицию в отсортированном массиве и проверяем случай отсутствия ключа.
    std::cout << "5! = " << factorial(5) << "\nF(10) = " << fibonacci(10)
              << "\nЦифр в -1203: " << countDigits(-1203)
              << "\nИндекс 7: " << binarySearch(values, 7, 0, 5)
              << "\nИндекс 2: " << binarySearch(values, 2, 0, 5) << '\n';
}
