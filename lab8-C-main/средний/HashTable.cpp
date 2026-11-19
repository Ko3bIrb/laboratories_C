#include "HashTable.h"


#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// Задаём кодировку консоли Windows для русских сообщений.
void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// Остаток задаёт первую ячейку. При коллизии идём по соседним ячейкам по кругу.
std::size_t HashTable::position(int key, std::size_t step) const {
    return (static_cast<unsigned>(key) % table.size() + step) % table.size();
}

// Удалённую ячейку пропускаем: за ней могут находиться другие ключи.
std::size_t HashTable::locate(int key) const {
    for (std::size_t step = 0; step < table.size(); ++step) {
        std::size_t index = position(key, step);
        const Entry& entry = table[index];
        // До этой ячейки никто не дошёл при вставке, поэтому искать дальше бессмысленно.
        if (entry.state == State::empty) break;
        if (entry.state == State::occupied && entry.key == key) return index;
    }
    return table.size();
}

// Число проб ограничено фиксированной ёмкостью таблицы.
HashTable::HashTable(std::size_t capacity) : table(capacity) {
    if (capacity == 0) throw std::invalid_argument("Нужна ненулевая ёмкость");
}

// Запоминаем свободную ячейку и ищем, есть ли уже такой ключ.
bool HashTable::insert(int key, int value) {
    std::size_t freeIndex = table.size();
    for (std::size_t step = 0; step < table.size(); ++step) {
        std::size_t index = position(key, step);
        Entry& entry = table[index];
        if (entry.state == State::occupied && entry.key == key) {
            entry.value = value;
            return true;
        }
        // Запоминаем место для вставки, но дочитываем цепочку: ключ мог быть добавлен раньше.
        if (entry.state != State::occupied && freeIndex == table.size()) freeIndex = index;
        // До этой ячейки никто не дошёл при вставке, поэтому искать дальше бессмысленно.
        if (entry.state == State::empty) break;
    }
    if (freeIndex == table.size()) return false;
    table[freeIndex] = {key, value, State::occupied};
    return true;
}

// optional отличает отсутствующий ключ от любого целого значения.
std::optional<int> HashTable::find(int key) const {
    std::size_t index = locate(key);
    if (index == table.size()) return std::nullopt;
    return table[index].value;
}

// Помечаем ячейку удалённой, чтобы сохранить цепочку проб.
bool HashTable::remove(int key) {
    std::size_t index = locate(key);
    if (index == table.size()) return false;
    table[index].state = State::deleted;
    return true;
}
