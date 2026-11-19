#pragma once
#include <string>
#include <vector>

bool compareStringSearch();

std::vector<std::size_t> prefixFunction(const std::string& pattern);
std::vector<std::size_t> findMatches(const std::string& text, const std::string& pattern,
                                     const std::vector<std::size_t>& pi, bool firstOnly = false);

void removeDuplicates(std::vector<int>& values);

void setupConsole();
