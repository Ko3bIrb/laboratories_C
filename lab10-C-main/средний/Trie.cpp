#include "Trie.h"


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

// После удаления конца слова убираем только больше не нужные ветви.
bool Trie::erase(Node& node, std::string_view suffix) {
    if (suffix.empty()) {
        if (!node.word) return false;
        node.word = false;
        return true;
    }
    auto child = node.children.find(suffix.front());
    if (child == node.children.end() || !erase(*child->second, suffix.substr(1))) return false;
    // Удаляем ребро лишь после того, как оно перестало вести к какому-либо слову.
    if (!child->second->word && child->second->children.empty()) node.children.erase(child);
    return true;
}

// Новая ветвь нужна только для ещё отсутствующей буквы.
void Trie::insert(std::string_view word) {
    Node* node = &root;
    for (char letter : word) {
        auto& child = node->children[letter];
        if (!child) child = std::make_unique<Node>();
        node = child.get();
    }
    node->word = true;
}

// Наличие пути недостаточно: на последнем узле должна стоять метка слова.
bool Trie::contains(std::string_view word) const {
    const Node* node = &root;
    for (char letter : word) {
        auto child = node->children.find(letter);
        if (child == node->children.end()) return false;
        node = child->second.get();
    }
    return node->word;
}

// Сообщаем, было ли слово в дереве.
bool Trie::remove(std::string_view word) { return erase(root, word); }
