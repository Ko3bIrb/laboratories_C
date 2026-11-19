#include "BinarySearch.h"
#include <iostream>

int main() {
    setupConsole();
    const std::vector<int> data{2, 4, 9, 16};
    std::cout << "Массив: 2 4 9 16\n";
    for (int key : {2, 9, 16, 7}) {
        int index = recursiveSearch(data, key, 0, static_cast<int>(data.size()));
        std::cout << "Ключ " << key << " — индекс " << index << '\n';
    }
}
