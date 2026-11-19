#include "Kmp.h"
#include <iostream>

int main() {
    setupConsole();
    const std::string text = "ababa aba", pattern = "aba";
    auto matches = findMatches(text, pattern, prefixFunction(pattern));
    std::cout << "Текст: " << text << "\nОбразец: " << pattern << "\nВсе вхождения:";
    for (auto position : matches) std::cout << ' ' << position;
    std::vector<int> numbers{4, 2, 4, 2, -3};
    std::cout << "\nМассив с дубликатами:";
    for (int value : numbers) std::cout << ' ' << value;
    removeDuplicates(numbers);
    std::cout << "\nМассив без дубликатов:";
    for (int value : numbers) std::cout << ' ' << value;
    std::cout << '\n';
}
