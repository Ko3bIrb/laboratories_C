#include "ListSort.h"
#include <iterator>

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

// Сливаем два списка, перенося узлы. При равенстве берём левый узел первым.
static void merge(std::forward_list<int>& left, std::forward_list<int>& right) {
    auto previous = left.before_begin();
    auto current = left.begin();
    while (current != left.end() && !right.empty()) {
        if (*current <= right.front()) {
            previous = current++;
        } else {
            // Переносим первый узел правого списка сразу перед current.
            left.splice_after(previous, right, right.before_begin());
            ++previous;
        }
    }
    left.splice_after(previous, right);
}

// Делим список пополам, сортируем обе половины и сливаем.
void mergeSort(std::forward_list<int>& data) {
    if (data.empty() || std::next(data.begin()) == data.end()) return;
    auto middle = data.begin();
    // fast идёт вдвое быстрее: когда он дойдёт до конца, middle будет у середины.
    auto fast = std::next(data.begin());
    while (fast != data.end() && std::next(fast) != data.end()) {
        ++middle;
        fast = std::next(fast, 2);
    }
    std::forward_list<int> right;
    right.splice_after(right.before_begin(), data, middle, data.end());
    mergeSort(data);
    mergeSort(right);
    merge(data, right);
}
