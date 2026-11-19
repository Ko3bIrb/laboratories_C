#pragma once
#include <vector>

bool showSortComparison();

struct Statistics { long long comparisons = 0; long long writes = 0; };
Statistics bubbleSort(std::vector<int>& values);
Statistics insertionSort(std::vector<int>& values);
Statistics selectionSort(std::vector<int>& values);

void setupConsole();
