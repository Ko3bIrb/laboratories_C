#include "Kmp.h"
#include <iostream>

int main() {
    setupConsole();
    const std::string text = "bac ababaca ababa", pattern = "ababa";
    auto pi = prefixFunction(pattern);
    auto first = findMatches(text, pattern, pi, true);
    std::cout << "Текст: " << text << "\nОбразец: " << pattern << "\nПрефикс-функция:";
    for (auto length : pi) std::cout << ' ' << length;
    if (first.empty()) {
        std::cout << "\nВхождений нет.\n";
    } else {
        std::cout << "\nПервое вхождение по индексу " << first.front() << '\n';
    }
}
