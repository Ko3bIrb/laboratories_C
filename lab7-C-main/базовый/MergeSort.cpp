#include "MergeSort.h"
#include <algorithm>

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

// Сортируем половины, затем сливаем их во вспомогательный массив.
static void mergeRange(std::vector<int>& data, std::vector<int>& buffer, int left, int right) {
    if (right - left < 2) return;
    int middle = left + (right - left) / 2;
    mergeRange(data, buffer, left, middle);
    mergeRange(data, buffer, middle, right);
    int i = left, j = middle;
    for (int k = left; k < right; ++k) {
        // При равенстве сначала берём элемент из левой половины.
        if (j == right || (i < middle && data[i] <= data[j])) buffer[k] = data[i++];
        else buffer[k] = data[j++];
    }
    std::copy(buffer.begin() + left, buffer.begin() + right, data.begin() + left);
}

// Один буфер используется для всех уровней рекурсии.
void mergeSort(std::vector<int>& data) {
    std::vector<int> buffer(data.size());
    mergeRange(data, buffer, 0, static_cast<int>(data.size()));
}
