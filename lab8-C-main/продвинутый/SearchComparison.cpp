#include "SearchComparison.h"
#include <chrono>
#include <iostream>
#include <random>

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

// Замеряем среднее время на один запрос и считаем найденные ключи.
template<class Find>
double measure(const std::vector<int>& queries, Find find, int& found) {
    found = 0;
    auto start = std::chrono::steady_clock::now();
    for (int key : queries) found += find(key);
    return std::chrono::duration<double, std::nano>(
        std::chrono::steady_clock::now() - start).count() / static_cast<double>(queries.size());
}

bool compareSearches() {
std::mt19937 random(8);
    std::cout << "\nN | Двоичный, нс/запрос | Хеш, нс/запрос | Найдено\n";
    for (int size : {1000, 10000, 100000}) {
        std::vector<int> data(size);
        HashTable table(static_cast<std::size_t>(size) * 2 + 1);
        for (int i = 0; i < size; ++i) {
            data[i] = i * 2;
            table.insert(data[i], i);
        }
        std::uniform_int_distribution<int> keys(0, size * 2 - 1);
        std::vector<int> queries(100000);
        for (int& key : queries) key = keys(random);
        int binaryFound, hashFound;
        double binary = measure(queries, [&](int key) { return iterativeSearch(data, key) >= 0; }, binaryFound);
        double hash = measure(queries, [&](int key) { return table.find(key).has_value(); }, hashFound);
        std::cout << size << " | " << binary << " | " << hash << " | " << binaryFound << '\n';
        if (binaryFound != hashFound) return false;
    }
    return true;
}

// В отсортированном массиве продолжаем поиск только в одной половине.
int recursiveSearch(const std::vector<int>& data, int key, int left, int right) {
    if (left == right) return -1;
    int middle = left + (right - left) / 2;
    if (data[middle] == key) return middle;
    if (key < data[middle]) return recursiveSearch(data, key, left, middle);
    return recursiveSearch(data, key, middle + 1, right);
}

// Итеративный вариант нужен для сравнения с хеш-таблицей.
int iterativeSearch(const std::vector<int>& data, int key) {
    int left = 0;
    int right = static_cast<int>(data.size());
    while (left < right) {
        int middle = left + (right - left) / 2;
        if (data[middle] == key) return middle;
        if (key < data[middle]) right = middle;
        else left = middle + 1;
    }
    return -1;
}
