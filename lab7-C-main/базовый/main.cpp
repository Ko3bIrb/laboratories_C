#include "MergeSort.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

int main() {
    setupConsole();
    std::mt19937 random(7);
    std::uniform_int_distribution<int> number(-5000, 5000);
    std::cout << "N | Слиянием, мкс | Результат\n";
    for (int size : {100, 1000, 10000}) {
        std::vector<int> data(size);
        for (int& value : data) value = number(random);
        auto expected = data;
        std::sort(expected.begin(), expected.end());
        auto start = std::chrono::steady_clock::now();
        mergeSort(data);
        double us = std::chrono::duration<double, std::micro>(
            std::chrono::steady_clock::now() - start).count();
        std::cout << size << " | " << us << " | " << (data == expected ? "верно" : "ошибка") << '\n';
        if (data != expected) return 1;
    }
}
