#include "Vector.h"

#include <iostream>

int main() {
    Vector<int> numbers;
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);

    std::cout << "Базовый уровень: Vector<int>\n";
    std::cout << "size = " << numbers.size()
              << ", capacity = " << numbers.capacity() << '\n';
    std::cout << "numbers[1] = " << numbers[1] << '\n';

    numbers.pop_back();
    std::cout << "После pop_back: size = " << numbers.size() << '\n';
    numbers.clear();
    std::cout << "После clear: empty = " << (numbers.empty() ? "true" : "false") << '\n';
    return 0;
}
