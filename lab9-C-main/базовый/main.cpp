#include "BinaryTree.h"
#include <iostream>

int main() {
    setupConsole();
    BinaryTree tree;
    tree.root = std::make_unique<Node>(10);
    tree.root->left = std::make_unique<Node>(20);
    tree.root->right = std::make_unique<Node>(4);
    tree.root->left->right = std::make_unique<Node>(7);

    // На одном дереве видно, в каком порядке каждый обход посещает те же узлы.
    std::cout << "Прямой: "; preOrder(tree.root.get());
    std::cout << "\nСимметричный: "; inOrder(tree.root.get());
    std::cout << "\nОбратный: "; postOrder(tree.root.get());
    std::cout << "\nПо уровням: "; levelOrder(tree.root.get());
    std::cout << "\nЛистьев: " << countLeaves(tree.root.get()) << "\n\n";
    printTree(tree.root.get());
}
