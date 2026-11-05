#include <iostream>
#include <string>
#include "Triple.h"

int main() {
    // Демонстрация шаблонной функции myMin с разными типами
    std::cout << "myMin(int): " << myMin(5, 10) << std::endl;
    std::cout << "myMin(double): " << myMin(3.14, 2.71) << std::endl;
    std::cout << "myMin(string): " << myMin(std::string("apple"), std::string("banana")) << std::endl;
    std::cout << "myMin(char): " << myMin('z', 'a') << std::endl;

    // Демонстрация шаблонного класса Triple
    Triple<int, double, std::string> t(42, 3.14, "hello");
    std::cout << "\nTriple: " << t.getFirst() << ", "
        << t.getSecond() << ", " << t.getThird() << std::endl;

    // Меняем первое значение через сеттер
    t.setFirst(100);
    std::cout << "After setFirst: " << t.getFirst() << std::endl;

    // Сравнение линейного и бинарного поиска
    std::vector<int> sizes = { 10000, 100000, 500000, 1000000 };
    std::cout << "\nSize\tLinear(us)\tBinary(us)" << std::endl;

    for (int n : sizes) {
        // Создаём отсортированный массив от 0 до n-1
        std::vector<int> arr(n);
        for (int i = 0; i < n; i++) arr[i] = i;

        // Ищем последний элемент
        int target = n - 1;

        // Замеряем время линейного поиска
        long long lt = measureTime([&]() { linearSearch(arr, target); });

        // Замеряем время бинарного поиска
        long long bt = measureTime([&]() { binarySearch(arr, target); });

        // Выводим результат для текущего размера
        std::cout << n << "\t" << lt << "\t\t" << bt << std::endl;
    }

    return 0;
}