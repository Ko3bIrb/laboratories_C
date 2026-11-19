#pragma once
#include <string>
#include <vector>

void removeDuplicates(std::vector<int>& values);

std::vector<std::size_t> prefixFunction(const std::string& pattern);
std::vector<std::size_t> findMatches(const std::string& text, const std::string& pattern,
                                     const std::vector<std::size_t>& pi, bool firstOnly = false);

void setupConsole();
