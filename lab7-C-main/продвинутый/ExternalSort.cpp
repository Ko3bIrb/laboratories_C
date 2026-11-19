#include "ExternalSort.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

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

namespace fs = std::filesystem;

class TemporaryRuns {
public:
    fs::path directory;

    // Создаём отдельную папку для промежуточных серий.
    TemporaryRuns() {
        auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        for (int attempt = 0; attempt < 100; ++attempt) {
            directory = fs::temp_directory_path() /
                ("lab7-runs-" + std::to_string(stamp) + "-" + std::to_string(attempt));
            if (fs::create_directory(directory)) return;
        }
        throw std::runtime_error("Не удалось создать временную папку");
    }

    // Убираем только временные серии нашей сортировки.
    ~TemporaryRuns() {
        std::error_code error;
        fs::remove_all(directory, error);
    }

    // Имя содержит номер прохода и номер серии.
    fs::path path(int pass, std::size_t index) const {
        return directory / (std::to_string(pass) + "-" + std::to_string(index) + ".txt");
    }
};

// Различаем конец файла и некорректное или слишком большое число.
static bool readNumber(std::istream& input, int& value) {
    input >> std::ws;
    if (input.eof() && !input.bad()) return false;
    if (!(input >> value)) throw std::runtime_error("В файле ожидалось целое число");
    return true;
}

// Сливаем две серии, держа в памяти только по одному числу из каждого файла.
static void mergeFiles(const fs::path& first, const fs::path& second, const fs::path& result) {
    std::ifstream left(first), right(second);
    std::ofstream output(result);
    if (!left || !right || !output) throw std::runtime_error("Ошибка открытия серии");
    int a = 0, b = 0;
    bool hasA = readNumber(left, a), hasB = readNumber(right, b);
    while (hasA || hasB) {
        if (!hasB || (hasA && a <= b)) {
            output << a << '\n';
            hasA = readNumber(left, a);
        } else {
            output << b << '\n';
            hasB = readNumber(right, b);
        }
    }
    output.close();
    if (!output) throw std::runtime_error("Ошибка записи серии");
}

// Сортируем вход по блокам, затем попарно сливаем серии до одного файла.
std::size_t externalSort(const fs::path& inputPath, const fs::path& outputPath) {
    TemporaryRuns files;
    constexpr std::size_t blockSize = 1024;
    std::size_t count = 0;
    {
        std::ifstream input(inputPath);
        if (!input) throw std::runtime_error("Не удалось открыть входной файл");
        std::vector<int> block;
        block.reserve(blockSize);
        int value = 0;
        // Последний блок может заполниться лишь частично.
        while (true) {
            block.clear();
            while (block.size() < blockSize && readNumber(input, value)) block.push_back(value);
            if (block.empty()) break;
            mergeSort(block);
            std::ofstream run(files.path(0, count++));
            for (int item : block) run << item << '\n';
            run.close();
            if (!run) throw std::runtime_error("Ошибка записи блока");
        }
    }
    if (count == 0) {
        std::ofstream output(outputPath);
        if (!output) throw std::runtime_error("Не удалось открыть выходной файл");
        return 0;
    }

    std::size_t originalCount = count;
    int pass = 0;
    while (count > 1) {
        for (std::size_t i = 0; i < count; i += 2) {
            auto first = files.path(pass, i);
            auto result = files.path(pass + 1, i / 2);
            if (i + 1 == count) {
                fs::rename(first, result);
            } else {
                auto second = files.path(pass, i + 1);
                mergeFiles(first, second, result);
                fs::remove(first);
                fs::remove(second);
            }
        }
        // Из каждой пары получаем одну серию. Последняя непарная серия тоже остаётся.
        count = (count + 1) / 2;
        ++pass;
    }
    fs::copy_file(files.path(pass, 0), outputPath, fs::copy_options::overwrite_existing);
    return originalCount;
}

// Сравниваем три сортировки на одинаковых массивах и проверяем результат.
void compare() {
    std::mt19937 random(7);
    std::uniform_int_distribution<int> number(-5000, 5000);
    std::cout << "N | Вид массива | Быстрая, мкс | Слиянием, мкс | Пирамидальная, мкс\n";
    for (int size : {100, 1000, 10000}) {
        std::vector<int> data(size);
        for (int& value : data) value = number(random);
        auto expected = data;
        std::sort(expected.begin(), expected.end());
        for (const char* kind : {"случайный", "почти упорядоченный", "обратный"}) {
            std::cout << size << " | " << kind;
            for (auto sort : {quickSort, mergeSort, heapSort}) {
                auto copy = data;
                auto start = std::chrono::steady_clock::now();
                sort(copy);
                double us = std::chrono::duration<double, std::micro>(
                    std::chrono::steady_clock::now() - start).count();
                if (copy != expected) throw std::logic_error("Неверный результат сортировки");
                std::cout << " | " << us;
            }
            std::cout << '\n';
            if (kind == std::string("случайный")) {
                data = expected;
                std::swap(data[0], data[1]);
            } else {
                data = expected;
                std::reverse(data.begin(), data.end());
            }
        }
    }
}

// Сортируем половины, затем сливаем их во вспомогательный массив.
static void mergeRange(std::vector<int>& data, std::vector<int>& buffer, int left, int right) {
    if (right - left < 2) return;
    int middle = left + (right - left) / 2;
    mergeRange(data, buffer, left, middle);
    mergeRange(data, buffer, middle, right);
    int i = left, j = middle;
    for (int k = left; k < right; ++k) {
        // При равенстве сначала берём элемент из левой половины.
        if (j == right || (i < middle && data[i] <= data[j])) buffer[k] = data[i++];
        else buffer[k] = data[j++];
    }
    std::copy(buffer.begin() + left, buffer.begin() + right, data.begin() + left);
}

// Один буфер используется для всех уровней рекурсии.
void mergeSort(std::vector<int>& data) {
    std::vector<int> buffer(data.size());
    mergeRange(data, buffer, 0, static_cast<int>(data.size()));
}

// Берём средний элемент за опорный. Меньшую часть сортируем рекурсивно.
static void quickRange(std::vector<int>& data, int left, int right) {
    while (left < right) {
        int pivot = data[left + (right - left) / 2];
        int i = left, j = right;
        while (i <= j) {
            while (data[i] < pivot) ++i;
            while (data[j] > pivot) --j;
            if (i <= j) std::swap(data[i++], data[j--]);
        }
        // Переход к большей части через цикл ограничивает глубину стека.
        if (j - left < right - i) {
            quickRange(data, left, j);
            left = i;
        } else {
            quickRange(data, i, right);
            right = j;
        }
    }
}

// Сортируем весь массив быстрой сортировкой.
void quickSort(std::vector<int>& data) {
    quickRange(data, 0, static_cast<int>(data.size()) - 1);
}

// Опускаем узел, пока он меньше одного из своих потомков.
static void siftDown(std::vector<int>& data, int root, int size) {
    while (root < size / 2) {
        int child = root * 2 + 1;
        if (child + 1 < size && data[child] < data[child + 1]) ++child;
        if (data[root] >= data[child]) break;
        std::swap(data[root], data[child]);
        root = child;
    }
}

// Строим max-heap и по одному переносим максимумы в конец.
void heapSort(std::vector<int>& data) {
    int size = static_cast<int>(data.size());
    for (int i = size / 2 - 1; i >= 0; --i) siftDown(data, i, size);
    for (int end = size - 1; end > 0; --end) {
        std::swap(data[0], data[end]);
        siftDown(data, 0, end);
    }
}
