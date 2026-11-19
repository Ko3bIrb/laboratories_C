#include "AvlTree.h"
#include <iostream>

int main() {
    setupConsole();
    const std::vector<std::vector<int>> orders{
        {30, 20, 10}, {10, 20, 30}, {30, 10, 20}, {10, 30, 20}
    };
    const char* turns[]{"Правый", "Левый", "Левый-правый", "Правый-левый"};
    for (std::size_t i = 0; i < orders.size(); ++i) {
        SearchTree avl(true);
        for (int key : orders[i]) avl.insert(key);
        std::cout << turns[i] << " поворот: высота " << avl.avlHeight()
                  << ", максимум " << avl.maximum().value() << '\n';
        if (avl.avlHeight() != 2 || avl.maximum() != 30) return 1;
    }

    SearchTree avl(true);
    for (int key : {30, 10, 20, 40, 50}) avl.insert(key);
    avl.removeFromAvl(50);
    avl.removeFromAvl(20);
    std::cout << "После удаления 50 и 20: максимум " << avl.maximum().value()
              << ", высота " << avl.avlHeight() << '\n';
    return (avl.contains(50) || avl.contains(20) || avl.maximum() != 40) ? 1 : 0;
}
