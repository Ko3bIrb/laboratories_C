#include "Knapsack.h"
#include <iostream>

int main() {
    setupConsole();
    const std::vector<Item> items{{2, 3}, {3, 4}, {4, 5}, {5, 8}};
    const int capacity = 10;
    auto table = knapsackTable(items, capacity);
    std::cout << "Вместимость: " << capacity << "\nМаксимальная ценность: "
              << table.back().back() << "\nТаблица рюкзака:\n";
    printTable(table);
}
