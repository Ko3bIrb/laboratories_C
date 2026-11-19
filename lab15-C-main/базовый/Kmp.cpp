#include "Kmp.h"


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
