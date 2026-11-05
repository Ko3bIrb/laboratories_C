#include "LinkedLists.h"

#include <chrono>
#include <iostream>
#include <string>

template <typename List>
void printList(const List& list, const std::string& title) {
    std::cout << title << "[";
    bool first = true;
    for (const auto& value : list) {
        if (!first) std::cout << ", ";
        std::cout << value;
        first = false;
    }
    std::cout << "]\n";
}

template <typename List>
long long measureFind(List& list, int value) {
    const auto start = std::chrono::steady_clock::now();
    auto found = list.find(value);
    const auto finish = std::chrono::steady_clock::now();
    (void)found;
    return std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
}

int main() {
    SinglyLinkedList<int> singly;
    singly.push_back(10);
    singly.push_back(20);
    singly.push_front(5);
    singly.push_back(30);

    printList(singly, "Односвязный список: ");
    std::cout << "Размер: " << singly.size() << '\n';
    std::cout << "Найдено 20: " << (singly.find(20) != singly.end() ? "да" : "нет") << '\n';
    singly.remove(20);
    singly.pop_front();
    printList(singly, "После remove(20) и pop_front: ");

    SinglyLinkedList<int> singlyCopy = singly;
    std::cout << "Сравнение списков: " << (singly == singlyCopy ? "равны" : "не равны") << '\n';
    singlyCopy.push_back(99);
    std::cout << "После изменения копии: " << (singly != singlyCopy ? "не равны" : "равны") << '\n';

    DoublyLinkedList<int> doubly;
    doubly.push_back(1);
    doubly.push_back(3);
    doubly.push_back(5);
    doubly.push_front(0);
    printList(doubly, "\nДвусвязный список: ");

    auto position = doubly.find(3);
    doubly.insert_after(position, 4);
    printList(doubly, "После insert_after(3, 4): ");
    doubly.erase(doubly.find(0));
    printList(doubly, "После удаления 0: ");

    DoublyLinkedList<int> first;
    DoublyLinkedList<int> second;
    first.push_back(1);
    first.push_back(4);
    first.push_back(7);
    second.push_back(2);
    second.push_back(3);
    second.push_back(8);
    first.merge(second);
    printList(first, "Слияние отсортированных списков: ");
    std::cout << "Второй список после merge пуст: " << (second.empty() ? "да" : "нет") << '\n';

    DoublyLinkedList<int> doublyCopy = doubly;
    std::cout << "Сравнение двусвязных списков: "
              << (doubly == doublyCopy ? "равны" : "не равны") << '\n';

    SinglyLinkedList<int> performanceSingly;
    DoublyLinkedList<int> performanceDoubly;
    for (int i = 0; i < 50000; ++i) {
        performanceSingly.push_back(i);
        performanceDoubly.push_back(i);
    }
    std::cout << "\nСравнение поиска элемента 49999 (мкс):\n";
    std::cout << "Односвязный: " << measureFind(performanceSingly, 49999) << '\n';
    std::cout << "Двусвязный:  " << measureFind(performanceDoubly, 49999) << '\n';

    try {
        SinglyLinkedList<int> empty;
        empty.pop_back();
    } catch (const std::underflow_error& error) {
        std::cout << "Проверка пустого списка: " << error.what() << '\n';
    }

    return 0;
}
