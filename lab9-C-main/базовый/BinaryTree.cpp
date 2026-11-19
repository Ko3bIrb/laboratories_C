#include "BinaryTree.h"
#include <algorithm>
#include <iostream>
#include <queue>

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

Node::Node(int key) : value(key) {}

// Прямой обход. Сначала узел, затем два поддерева.
void preOrder(const Node* node) {
    if (!node) return;
    std::cout << node->value << ' ';
    preOrder(node->left.get());
    preOrder(node->right.get());
}

// Симметричный обход. Узел печатается между левым и правым поддеревьями.
void inOrder(const Node* node) {
    if (!node) return;
    inOrder(node->left.get());
    std::cout << node->value << ' ';
    inOrder(node->right.get());
}

// Обратный обход. Узел печатается после обоих поддеревьев.
void postOrder(const Node* node) {
    if (!node) return;
    postOrder(node->left.get());
    postOrder(node->right.get());
}

// Посещаем уровни слева направо, используя очередь.
void levelOrder(const Node* node) {
    if (!node) return;
    std::queue<const Node*> pending;
    pending.push(node);
    while (!pending.empty()) {
        const Node* current = pending.front();
        pending.pop();
        std::cout << current->value << ' ';
        if (current->left) pending.push(current->left.get());
        if (current->right) pending.push(current->right.get());
    }
}

// Листом считаем узел без потомков.
int countLeaves(const Node* node) {
    if (!node) return 0;
    if (!node->left && !node->right) return 1;
    return countLeaves(node->left.get()) + countLeaves(node->right.get());
}

// Высота измеряется в узлах. У пустого дерева она равна нулю.
int height(const Node* node) {
    if (!node) return 0;
    return 1 + std::max(height(node->left.get()), height(node->right.get()));
}

// Печатаем дерево боком, чтобы были видны связи между уровнями.
void printTree(const Node* node, int depth) {
    if (!node) return;
    printTree(node->right.get(), depth + 1);
    for (int i = 0; i < depth; ++i) std::cout << "    ";
    std::cout << node->value << '\n';
    printTree(node->left.get(), depth + 1);
}
