#include "BinarySearch.h"


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

// В отсортированном массиве продолжаем поиск только в одной половине.
int recursiveSearch(const std::vector<int>& data, int key, int left, int right) {
    if (left == right) return -1;
    int middle = left + (right - left) / 2;
    if (data[middle] == key) return middle;
    if (key < data[middle]) return recursiveSearch(data, key, left, middle);
    return recursiveSearch(data, key, middle + 1, right);
}
