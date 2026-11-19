#include "ListSort.h"
#include <iostream>

// Печатаем все значения списка в одну строку.
void print(const std::forward_list<int>& data) {
    for (int value : data) std::cout << value << ' ';
    std::cout << '\n';
}

int main() {
    setupConsole();
    std::forward_list<int> data{6, -1, 4, 4, 2, 0, 8, -3};
    std::cout << "До: "; print(data);
    mergeSort(data);
    std::cout << "После: "; print(data);
    return data == std::forward_list<int>{-3, -1, 0, 2, 4, 4, 6, 8} ? 0 : 1;
}
