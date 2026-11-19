#pragma once
#include <vector>

bool compareFibonacci();

bool showSortComparison();

unsigned long long factorial(unsigned n);
unsigned long long fibonacci(unsigned n);
int countDigits(long long number);
int binarySearch(const std::vector<int>& values, int key, int left, int right);

struct Statistics { long long comparisons = 0; long long writes = 0; };
Statistics bubbleSort(std::vector<int>& values);
Statistics insertionSort(std::vector<int>& values);
Statistics selectionSort(std::vector<int>& values);

void setupConsole();
