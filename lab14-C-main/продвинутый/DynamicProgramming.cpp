#include "DynamicProgramming.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

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

// Соседние байты UTF-8, начинающиеся с 10, продолжают текущий символ.
std::vector<std::string_view> utf8Characters(const std::string& text) {
    std::vector<std::string_view> characters;
    std::string_view view(text);
    std::size_t start = 0;
    while (start < view.size()) {
        std::size_t end = start + 1;
        while (end < view.size() && (static_cast<unsigned char>(view[end]) & 0xc0) == 0x80) ++end;
        characters.push_back(view.substr(start, end - start));
        start = end;
    }
    return characters;
}

// table[i][j] хранит длину общей подпоследовательности двух префиксов.
LcsResult longestCommonSubsequence(const std::vector<std::string_view>& x,
                                   const std::vector<std::string_view>& y) {
    LcsResult result{Table(x.size() + 1, std::vector<long long>(y.size() + 1)), {}};
    auto& dp = result.table;
    for (std::size_t i = 1; i <= x.size(); ++i) {
        for (std::size_t j = 1; j <= y.size(); ++j) {
            if (x[i - 1] == y[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    // Идём от правого нижнего угла к началу и собираем ответ в обратном порядке.
    std::size_t i = x.size(), j = y.size();
    while (i > 0 && j > 0) {
        if (x[i - 1] == y[j - 1]) {
            // Равные последние символы добавляем в ответ и убираем из обоих префиксов.
            result.sequence.push_back(x[i - 1]);
            --i;
            --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    std::reverse(result.sequence.begin(), result.sequence.end());
    return result;
}

// Идём по строкам таблицы снизу вверх и восстанавливаем один оптимальный набор.
std::vector<int> selectedItems(const std::vector<Item>& items, const Table& dp, int capacity) {
    std::vector<int> selected;
    int w = capacity;
    for (std::size_t i = items.size(); i > 0; --i) {
        // Если результат не изменился, существует оптимальный набор без этого предмета.
        if (dp[i][w] == dp[i - 1][w]) continue;
        selected.push_back(static_cast<int>(i) - 1);
        w -= items[i - 1].weight;
    }
    std::reverse(selected.begin(), selected.end());
    return selected;
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
