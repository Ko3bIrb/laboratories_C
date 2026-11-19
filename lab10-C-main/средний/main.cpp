#include "Trie.h"
#include <iostream>

int main() {
    setupConsole();
    Trie trie;
    for (const char* word : {"car", "cart", "cat"}) trie.insert(word);
    std::cout << std::boolalpha << "После вставки car: " << trie.contains("car")
              << "; ca — целое слово: " << trie.contains("ca") << '\n'
              << "Удаляем car: " << trie.remove("car") << '\n'
              << "Поиск car: " << trie.contains("car")
              << "; cart: " << trie.contains("cart")
              << "; cat: " << trie.contains("cat") << '\n';
}
