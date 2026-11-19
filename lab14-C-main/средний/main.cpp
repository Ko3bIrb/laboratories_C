#include "Knapsack.h"
#include <iostream>

int main() {
    setupConsole();
    const std::vector<Item> items{{2, 3}, {3, 4}, {4, 5}, {5, 8}};
    const int capacity = 10;
    auto table = knapsackTable(items, capacity);
    auto chosen = selectedItems(items, table, capacity);
    std::cout << "Максимальная ценность: " << table.back().back() << '\n';
    for (int i : chosen) {
        std::cout << "Предмет " << i << ": вес " << items[i].weight
                  << ", ценность " << items[i].value << '\n';
    }
    std::cout << "Таблица рюкзака:\n";
    printTable(table);
}
