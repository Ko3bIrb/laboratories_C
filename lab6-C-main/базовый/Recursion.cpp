#include "Recursion.h"


#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// Задаём кодировку консоли Windows для русских сообщений.
void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// Умножаем n на факториал предыдущего числа. Для 0 и 1 ответ уже известен.
unsigned long long factorial(unsigned n) {
    if (n < 2) return 1;
    return n * factorial(n - 1);
}

// Число Фибоначчи складывается из двух предыдущих.
unsigned long long fibonacci(unsigned n) {
    if (n < 2) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Деление на 10 убирает последнюю цифру, даже если число отрицательное.
int countDigits(long long number) {
    // Однозначное число, включая ноль, даёт одну цифру.
    if (number / 10 == 0) return 1;
    return 1 + countDigits(number / 10);
}

// Ищем в полуинтервале [left, right), чтобы пустой диапазон имел left == right.
int binarySearch(const std::vector<int>& values, int key, int left, int right) {
    if (left == right) return -1;
    int middle = left + (right - left) / 2;
    if (values[middle] == key) return middle;
    if (key < values[middle]) return binarySearch(values, key, left, middle);
    return binarySearch(values, key, middle + 1, right);
}
