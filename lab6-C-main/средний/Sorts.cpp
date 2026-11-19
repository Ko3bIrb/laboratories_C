#include "Sorts.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>

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

// На каждом проходе самый большой из оставшихся элементов уходит вправо.
Statistics bubbleSort(std::vector<int>& values) {
    Statistics stats;
    for (std::size_t end = values.size(); end > 1; --end) {
        bool changed = false;
        for (std::size_t i = 1; i < end; ++i) {
            ++stats.comparisons;
            if (values[i - 1] > values[i]) {
                std::swap(values[i - 1], values[i]);
                stats.writes += 2;
                changed = true;
            }
        }
        // Если за весь проход обменов не было, оставшаяся часть уже упорядочена.
        if (!changed) break;
    }
    return stats;
}

// Вставляем очередной элемент на его место в уже отсортированной части.
Statistics insertionSort(std::vector<int>& values) {
    Statistics stats;
    for (std::size_t i = 1; i < values.size(); ++i) {
        int value = values[i];
        std::size_t position = i;
        while (position > 0) {
            ++stats.comparisons;
            if (values[position - 1] <= value) break;
            // Освобождаем место для value, сдвигая больший элемент на одну позицию.
            values[position] = values[position - 1];
            ++stats.writes;
            --position;
        }
        if (position != i) {
            values[position] = value;
            ++stats.writes;
        }
    }
    return stats;
}

// Выбираем минимум оставшейся части и переносим его в начало.
Statistics selectionSort(std::vector<int>& values) {
    Statistics stats;
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::size_t minimum = i;
        for (std::size_t j = i + 1; j < values.size(); ++j) {
            ++stats.comparisons;
            if (values[j] < values[minimum]) minimum = j;
        }
        if (minimum != i) {
            std::swap(values[i], values[minimum]);
            stats.writes += 2;
        }
    }
    return stats;
}

// Копируем данные до замера, чтобы сравнить сортировки на одном массиве.
bool compareSort(const char* name, Statistics (*sort)(std::vector<int>&),
                 const std::vector<int>& input) {
    auto values = input;
    auto start = std::chrono::steady_clock::now();
    Statistics stats = sort(values);
    double us = std::chrono::duration<double, std::micro>(
        std::chrono::steady_clock::now() - start).count();
    bool sorted = std::is_sorted(values.begin(), values.end());
    std::cout << input.size() << " | " << name << " | " << stats.comparisons << " | "
              << stats.writes << " | " << us << " | " << (sorted ? "да" : "нет") << '\n';
    return sorted;
}

// Делаем несколько соседних перестановок. Каждая сортировка получает свою копию.
bool showSortComparison() {
    std::cout << "N | Сортировка | Сравнения | Записи | мкс | Порядок\n";
    for (int size : {1000, 3000, 6000}) {
        std::vector<int> values(size);
        std::iota(values.begin(), values.end(), 0);
        for (int i = 10; i + 1 < size; i += 40) std::swap(values[i], values[i + 1]);
        if (!compareSort("Пузырьковая", bubbleSort, values)
            || !compareSort("Вставками", insertionSort, values)
            || !compareSort("Выбором", selectionSort, values)) return false;
    }
    return true;
}
