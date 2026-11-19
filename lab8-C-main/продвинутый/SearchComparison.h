#pragma once
#include <optional>
#include <stdexcept>
#include <vector>

bool compareSearches();

int recursiveSearch(const std::vector<int>& data, int key, int left, int right);
int iterativeSearch(const std::vector<int>& data, int key);

class HashTable {
    enum class State { empty, occupied, deleted };
    struct Entry { int key = 0; int value = 0; State state = State::empty; };
    std::vector<Entry> table;

    std::size_t position(int key, std::size_t step) const;

    std::size_t locate(int key) const;

public:
    explicit HashTable(std::size_t capacity);

    bool insert(int key, int value);

    std::optional<int> find(int key) const;

    bool remove(int key);
};

void setupConsole();
