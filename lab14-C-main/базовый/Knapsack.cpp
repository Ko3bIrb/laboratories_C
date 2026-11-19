#include "Knapsack.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

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

// table[i][w] хранит наибольшую ценность для первых i предметов и вместимости w.
Table knapsackTable(const std::vector<Item>& items, int capacity) {
    Table dp(items.size() + 1, std::vector<long long>(capacity + 1));
    for (std::size_t i = 1; i <= items.size(); ++i) {
        Item item = items[i - 1];
        for (int w = 0; w <= capacity; ++w) {
            dp[i][w] = dp[i - 1][w];
            if (item.weight <= w) {
                // Строка i - 1 исключает повторный выбор текущего предмета.
                dp[i][w] = std::max(dp[i][w], dp[i - 1][w - item.weight] + item.value);
            }
        }
    }
    return dp;
}

// Подписываем столбцы и строки номерами префиксов или значениями вместимости.
void printTable(const Table& table) {
    int width = static_cast<int>(std::max(std::to_string(table.back().back()).size(),
                                          std::to_string(table.front().size()).size())) + 2;
    std::cout << std::setw(width) << "i/j";
    for (std::size_t j = 0; j < table.front().size(); ++j) std::cout << std::setw(width) << j;
    std::cout << '\n';
    for (std::size_t i = 0; i < table.size(); ++i) {
        std::cout << std::setw(width) << i;
        for (auto value : table[i]) std::cout << std::setw(width) << value;
        std::cout << '\n';
    }
}
