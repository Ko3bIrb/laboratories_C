#pragma once
#include <filesystem>
#include <vector>

std::size_t externalSort(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath);

void compare();

void mergeSort(std::vector<int>& data);
void quickSort(std::vector<int>& data);
void heapSort(std::vector<int>& data);

void setupConsole();
