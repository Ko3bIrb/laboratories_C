#pragma once
#include <string>
#include <string_view>
#include <vector>

using Table = std::vector<std::vector<long long>>;
void printTable(const Table& table);

struct LcsResult { Table table; std::vector<std::string_view> sequence; };
std::vector<std::string_view> utf8Characters(const std::string& text);
LcsResult longestCommonSubsequence(const std::vector<std::string_view>& x,
                                   const std::vector<std::string_view>& y);

struct Item { int weight, value; };
Table knapsackTable(const std::vector<Item>& items, int capacity);

std::vector<int> selectedItems(const std::vector<Item>& items, const Table& dp, int capacity);

void setupConsole();
