#include "Vector.h"

#include <iostream>

void print(const Vector<int>& values) {
    for (std::size_t i = 0; i < values.size(); ++i) std::cout << values[i] << ' ';
    std::cout << '\n';
}

int main() {
    Vector<int> values;
    for (int i = 1; i <= 5; ++i) values.push_back(i);

    std::cout << "Средний уровень: Vector<int>\n";
    std::cout << "Исходный массив: ";
    print(values);
    values.reverse();
    std::cout << "После reverse: ";
    print(values);

    Vector<int> copy(values);
    Vector<int> moved(std::move(copy));
    std::cout << "После копирования и перемещения: ";
    print(moved);
    std::cout << "Размер moved-from объекта: " << copy.size() << '\n';
    return 0;
}
