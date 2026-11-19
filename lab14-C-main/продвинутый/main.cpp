#include "DynamicProgramming.h"
#include <iostream>
#include <limits>

int main() {
    setupConsole();
    constexpr std::size_t maxCells = 100000;
    int count, capacity;
    std::cout << "Количество предметов и вместимость: ";
    if (!(std::cin >> count >> capacity) || count < 0 || capacity < 0
        || (static_cast<long long>(count) + 1) * (static_cast<long long>(capacity) + 1) > maxCells) {
        std::cerr << "Нужны неотрицательные размеры. В таблице допустимо до 100000 клеток.\n";
        return 1;
    }
    std::vector<Item> items(count);
    std::cout << "Введите вес и ценность каждого предмета.\n";
    for (Item& item : items) {
        if (!(std::cin >> item.weight >> item.value) || item.weight < 0 || item.value < 0) {
            std::cerr << "Вес и ценность должны быть неотрицательными целыми числами.\n";
            return 1;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string x, y;
    std::cout << "Строка X: ";
    if (!std::getline(std::cin, x)) return 1;
    std::cout << "Строка Y: ";
    if (!std::getline(std::cin, y)) return 1;
    auto first = utf8Characters(x), second = utf8Characters(y);
    if (first.size() + 1 > maxCells / (second.size() + 1)) {
        std::cerr << "Таблица LCS превышает 100000 клеток.\n";
        return 1;
    }

    auto bag = knapsackTable(items, capacity);
    auto selected = selectedItems(items, bag, capacity);
    std::cout << "\nМаксимальная ценность: " << bag.back().back()
              << "\nПредметы (индексы от 0):";
    for (int i : selected) std::cout << ' ' << i;
    if (selected.empty()) std::cout << " нет";
    std::cout << "\nТаблица рюкзака (строки — предметы, столбцы — вместимость)\n";
    printTable(bag);

    auto lcs = longestCommonSubsequence(first, second);
    std::cout << "\nДлина LCS: " << lcs.sequence.size() << "\nLCS: ";
    for (auto character : lcs.sequence) std::cout << character;
    std::cout << "\nТаблица LCS (строки — префиксы X, столбцы — префиксы Y)\n";
    printTable(lcs.table);
}
