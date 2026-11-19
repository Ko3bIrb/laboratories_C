#include "SearchTree.h"
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

// Идём влево, к текущему узлу и вправо, считая посещённые узлы.
std::optional<int> SearchTree::kth(const Node* node, int& remaining) {
    if (!node) return std::nullopt;
    if (auto result = kth(node->left.get(), remaining)) return result;
    // Левое поддерево уже пройдено. Теперь учитываем сам узел.
    if (--remaining == 0) return node->value;
    return kth(node->right.get(), remaining);
}

// При двух потомках переносим сюда минимальный ключ правого поддерева.
bool SearchTree::erase(std::unique_ptr<Node>& node, int key) {
    if (!node) return false;
    if (key < node->value) return erase(node->left, key);
    if (key > node->value) return erase(node->right, key);
    if (!node->left) {
        auto child = std::move(node->right);
        node = std::move(child);
    } else if (!node->right) {
        auto child = std::move(node->left);
        node = std::move(child);
    } else {
        // Наименьший ключ справа сохраняет порядок BST после замены.
        const Node* successor = node->right.get();
        while (successor->left) successor = successor->left.get();
        node->value = successor->value;
        erase(node->right, node->value);
    }
    return true;
}

// Ключи меньше текущего идут влево, больше идут вправо.
void SearchTree::insert(int key) {
    auto* link = &root;
    while (*link) {
        if (key == (*link)->value) return;
        link = key < (*link)->value ? &(*link)->left : &(*link)->right;
    }
    *link = std::make_unique<Node>(key);
}

// Сравнение ключей даёт нужное направление на каждом шаге.
bool SearchTree::search(int key) const {
    const Node* node = root.get();
    while (node && node->value != key) {
        node = key < node->value ? node->left.get() : node->right.get();
    }
    return node != nullptr;
}

// Первый наименьший элемент имеет номер 1.
std::optional<int> SearchTree::kthSmallest(int k) const {
    if (k <= 0) return std::nullopt;
    return kth(root.get(), k);
}

// Идём до крайнего узла с указанной стороны.
std::optional<int> SearchTree::extreme(bool maximum) const {
    const Node* node = root.get();
    if (!node) return std::nullopt;
    while (const Node* next = maximum ? node->right.get() : node->left.get()) node = next;
    return node->value;
}

// Делегируем удаление функции, которая умеет менять ссылку на узел.
bool SearchTree::remove(int key) {
    return erase(root, key);
}

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
