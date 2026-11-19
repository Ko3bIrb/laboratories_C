#include "TextSearch.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string_view>

int main(int argc, char* argv[]) {
    setupConsole();
    if (argc == 2 && std::string_view(argv[1]) == "--compare") return compareStringSearch() ? 0 : 1;
    if (argc > 2) {
        std::cerr << "Укажите один файл с текстом.\n";
        return 1;
    }
    std::string text, pattern, numbers;
    if (argc == 2) {
        std::ifstream input(std::filesystem::u8path(argv[1]), std::ios::binary);
        if (!input) {
            std::cerr << "Не удалось открыть файл.\n";
            return 1;
        }
        text.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    } else {
        std::cout << "Текст: ";
        if (!std::getline(std::cin, text)) return 1;
    }

    std::cout << "Образец: ";
    if (!std::getline(std::cin, pattern) || pattern.empty()) {
        std::cerr << "Образец должен содержать хотя бы один символ.\n";
        return 1;
    }
    auto pi = prefixFunction(pattern);
    auto matches = findMatches(text, pattern, pi);
    std::cout << "Префикс-функция:";
    for (auto length : pi) std::cout << ' ' << length;
    std::cout << '\n';
    if (matches.empty()) {
        std::cout << "Вхождений нет.\n";
    } else {
        std::cout << "Первое вхождение: " << matches.front() << "\nВсе позиции (смещения байтов от 0):";
        for (auto position : matches) std::cout << ' ' << position;
        std::cout << '\n';
    }

    std::cout << "Введите целые числа через пробел: ";
    if (!std::getline(std::cin, numbers)) return 1;
    std::istringstream input(numbers);
    std::vector<int> values;
    int number;
    while (true) {
        input >> std::ws;
        if (input.eof()) break;
        if (!(input >> number)) {
            std::cerr << "В массиве ожидалось целое число.\n";
            return 1;
        }
        values.push_back(number);
    }
    removeDuplicates(values);
    std::cout << "Массив без дубликатов:";
    for (int value : values) std::cout << ' ' << value;
    std::cout << '\n';
}
