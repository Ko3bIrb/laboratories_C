#include "SearchTree.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
    setupConsole();

    std::vector<int> keys(99);
    std::iota(keys.begin(), keys.end(), 1);
    std::mt19937 random(9);
    std::shuffle(keys.begin(), keys.end(), random);
    SearchTree bst;
    for (int i = 0; i < 20; ++i) bst.insert(keys[i]);
    std::cout << "BST по возрастанию: "; inOrder(bst.root.get());
    std::cout << "\nМинимум: " << bst.extreme(false).value()
              << ", максимум: " << bst.extreme(true).value()
              << ", высота: " << height(bst.root.get())
              << "\n3-й наименьший: " << bst.kthSmallest(3).value()
              << "\nКорень " << keys[0] << " найден: " << (bst.search(keys[0]) ? "да" : "нет")
              << "\nВведите ключ для удаления: ";
    int key;
    if (!(std::cin >> key)) {
        std::cerr << "Ожидалось целое число.\n";
        return 1;
    }
    std::cout << key << (bst.remove(key) ? " удалён.\n" : " отсутствует.\n");
    printTree(bst.root.get());
}
