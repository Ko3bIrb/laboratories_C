#include "Vector.h"

#include <iostream>
#include <numeric>
#include <string>

template <typename T>
void printVector(const Vector<T>& values, const std::string& title) {
    std::cout << title;
    for (const T& value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}

int main() {
    Vector<int> numbers;
    std::cout << "Пустой вектор: size=" << numbers.size()
              << ", capacity=" << numbers.capacity() << '\n';

    for (int i = 1; i <= 5; ++i) {
        numbers.push_back(i * 10);
        std::cout << "push_back(" << i * 10 << "): size=" << numbers.size()
                  << ", capacity=" << numbers.capacity() << '\n';
    }

    printVector(numbers, "Элементы через range-based for: ");
    std::cout << "numbers[2] = " << numbers[2] << '\n';

    // Проверка const_iterator из задания.
    const Vector<int> constNumbers = numbers;
    int sum = std::accumulate(constNumbers.cbegin(), constNumbers.cend(), 0);
    std::cout << "Обход через const_iterator: ";
    for (Vector<int>::const_iterator it = constNumbers.cbegin();
         it != constNumbers.cend(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << "\nСумма элементов: " << sum << '\n';

    numbers.pop_back();
    printVector(numbers, "После pop_back: ");
    numbers.resize(7, 99);
    printVector(numbers, "После resize(7, 99): ");

    Vector<int> copied(numbers);
    Vector<int> moved(std::move(copied));
    printVector(moved, "После копирования и перемещения: ");
    std::cout << "Состояние moved-from объекта: size=" << copied.size() << '\n';

    try {
        std::cout << numbers[100] << '\n';
    } catch (const std::out_of_range& error) {
        std::cout << "Проверка ошибки: " << error.what() << '\n';
    }

    numbers.clear();
    std::cout << "После clear: size=" << numbers.size()
              << ", empty=" << (numbers.empty() ? "true" : "false") << '\n';
    return 0;
}
