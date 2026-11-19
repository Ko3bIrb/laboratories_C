#include "HashTable.h"
#include <iostream>

int main() {
    setupConsole();
    HashTable table(5);
    // Ключи 4, 9 и 14 дают одну стартовую позицию.
    table.insert(4, 40);
    table.insert(9, 90);
    std::cout << "Ключ 9 после коллизии: " << table.find(9).value() << '\n';
    table.remove(4);
    std::cout << "Ключ 9 после удаления 4: " << table.find(9).value() << '\n';
    table.insert(9, 99);
    table.insert(14, 140);
    std::cout << "Обновлённое значение 9: " << table.find(9).value()
              << "\nЗначение 14: " << table.find(14).value()
              << "\nКлюч 4 найден: " << (table.find(4) ? "да" : "нет") << '\n';
}
