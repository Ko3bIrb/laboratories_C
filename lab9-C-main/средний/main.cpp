#include "SearchTree.h"
#include <iostream>

int main() {
    setupConsole();
    SearchTree tree;
    for (int key : {8, 3, 10, 1, 6, 14, 4, 7, 13}) tree.insert(key);
    std::cout << std::boolalpha << "Поиск 6: " << tree.search(6)
              << "\nПоиск 9: " << tree.search(9) << "\nПо порядку: ";
    inOrder(tree.root.get());
    std::cout << "\n4-й наименьший: " << tree.kthSmallest(4).value()
              << "\n10-й существует: " << tree.kthSmallest(10).has_value() << '\n';
}
