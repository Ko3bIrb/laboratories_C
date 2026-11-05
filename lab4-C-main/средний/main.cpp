#include "DoublyLinkedList.h"

#include <iostream>

int main() {
    DoublyLinkedList<int> first;
    DoublyLinkedList<int> second;
    first.push_back(1);
    first.push_back(4);
    first.push_back(7);
    second.push_back(2);
    second.push_back(3);
    second.push_back(8);

    std::cout << "Средний уровень: двусвязный список\n";
    std::cout << "Первый список: "; first.print();
    std::cout << "Второй список: "; second.print();
    first.merge(second);
    std::cout << "После merge: "; first.print();
    std::cout << "Обратный порядок: "; first.printReverse();
    std::cout << "Второй список пуст: " << (second.empty() ? "да" : "нет") << '\n';
    return 0;
}
