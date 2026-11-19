#pragma once
#include <vector>

using Table = std::vector<std::vector<long long>>;
void printTable(const Table& table);

struct Item { int weight, value; };
Table knapsackTable(const std::vector<Item>& items, int capacity);

std::vector<int> selectedItems(const std::vector<Item>& items, const Table& dp, int capacity);

void setupConsole();
