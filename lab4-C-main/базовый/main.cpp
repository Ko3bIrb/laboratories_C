#include "SinglyLinkedList.h"

#include <iostream>

int main() {
    SinglyLinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    std::cout << "Базовый уровень: односвязный список\n";
    std::cout << "Список: ";
    list.print();
    std::cout << "Количество элементов: " << list.size() << '\n';
    list.pop_back();
    std::cout << "После pop_back: ";
    list.print();
    return 0;
}
