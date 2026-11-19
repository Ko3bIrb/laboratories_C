#include "TextSearch.h"
#include <algorithm>
#include <chrono>
#include <iostream>

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

// Проверяем совпадение с каждой возможной позиции текста.
static std::vector<std::size_t> naiveSearch(const std::string& text, const std::string& pattern) {
    std::vector<std::size_t> positions;
    for (std::size_t i = 0; i + pattern.size() <= text.size(); ++i) {
        if (text.compare(i, pattern.size(), pattern) == 0) positions.push_back(i);
    }
    return positions;
}

bool compareStringSearch() {
    using Clock = std::chrono::steady_clock;
    using Us = std::chrono::duration<double, std::micro>;
    std::string pattern(512, 'a');
    pattern.back() = 'b';
    std::cout << "Размер текста | Наивный, мкс | КМП, мкс | Найдено\n";
    for (std::size_t size : {10000, 40000, 100000}) {
        // Почти в каждой позиции наивный поиск сравнивает длинный префикс.
        std::string text(size, 'a');
        text.back() = 'b';
        const auto before = Clock::now();
        const auto naive = naiveSearch(text, pattern);
        const auto middle = Clock::now();
        const auto matches = findMatches(text, pattern, prefixFunction(pattern));
        const auto after = Clock::now();
        if (naive != matches) return false;
        std::cout << size << " | " << Us(middle - before).count()
                  << " | " << Us(after - middle).count() << " | " << matches.size() << '\n';
    }
    return true;
}

// pi[i] — длина наибольшего собственного префикса, совпадающего с суффиксом до i.
std::vector<std::size_t> prefixFunction(const std::string& pattern) {
    std::vector<std::size_t> pi(pattern.size());
    for (std::size_t i = 1; i < pattern.size(); ++i) {
        std::size_t matched = pi[i - 1];
        while (matched > 0 && pattern[i] != pattern[matched]) matched = pi[matched - 1];
        if (pattern[i] == pattern[matched]) ++matched;
        pi[i] = matched;
    }
    return pi;
}

// После полного совпадения сохраняем общий суффикс, чтобы найти и перекрывающиеся вхождения.
std::vector<std::size_t> findMatches(const std::string& text, const std::string& pattern,
                                 const std::vector<std::size_t>& pi, bool firstOnly) {
    std::vector<std::size_t> positions;
    if (pattern.empty()) return positions;
    std::size_t matched = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        // Текст не перематываем: сокращаем только уже совпавшую часть образца.
        while (matched > 0 && text[i] != pattern[matched]) matched = pi[matched - 1];
        if (text[i] == pattern[matched]) ++matched;
        if (matched == pattern.size()) {
            positions.push_back(i + 1 - pattern.size());
            // На базовом уровне достаточно позиции первого совпадения.
            if (firstOnly) break;
            matched = pi[matched - 1];
        }
    }
    return positions;
}

// Сортировка ставит равные числа рядом. unique сдвигает различные числа в начало.
void removeDuplicates(std::vector<int>& values) {
    std::sort(values.begin(), values.end());
    // unique уплотняет разные значения и возвращает начало хвоста, который стираем.
    values.erase(std::unique(values.begin(), values.end()), values.end());
}
