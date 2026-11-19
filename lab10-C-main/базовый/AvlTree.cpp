#include "AvlTree.h"


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

// Индекс -1 обозначает пустое поддерево.
int SearchTree::height(int node) const { return node == -1 ? 0 : nodes[node].height; }

// Высота меняется только у узлов, чьи поддеревья изменились.
void SearchTree::updateHeight(int node) {
    nodes[node].height = 1 + std::max(height(nodes[node].left), height(nodes[node].right));
}

// Положительное число означает, что левая ветвь выше.
int SearchTree::balance(int node) const {
    return height(nodes[node].left) - height(nodes[node].right);
}

// Поднимаем правого потомка, сохраняя порядок ключей.
int SearchTree::rotateLeft(int node) {
    int top = nodes[node].right;
    nodes[node].right = nodes[top].left;
    // Старый корень становится левым потомком. Внутренняя ветвь уже перенесена.
    nodes[top].left = node;
    updateHeight(node);
    updateHeight(top);
    return top;
}

// Поднимаем левого потомка, сохраняя порядок ключей.
int SearchTree::rotateRight(int node) {
    int top = nodes[node].left;
    nodes[node].left = nodes[top].right;
    nodes[top].right = node;
    updateHeight(node);
    updateHeight(top);
    return top;
}

// После вставки проверяем баланс. Если ветвь изгибается, нужны два поворота.
int SearchTree::rebalance(int node) {
    updateHeight(node);
    if (balance(node) > 1) {
        int left = nodes[node].left;
        // Если левая ветвь наклонена вправо, сначала выпрямляем её левым поворотом.
        if (balance(left) < 0) nodes[node].left = rotateLeft(left);
        return rotateRight(node);
    }
    if (balance(node) < -1) {
        int right = nodes[node].right;
        if (balance(right) > 0) nodes[node].right = rotateRight(right);
        return rotateLeft(node);
    }
    return node;
}

// AVL меняется снизу вверх, поэтому после рекурсии можно восстановить баланс.
int SearchTree::insertAvl(int node, int key) {
    if (node == -1) return addNode(key);
    if (key < nodes[node].key) nodes[node].left = insertAvl(nodes[node].left, key);
    else if (key > nodes[node].key) nodes[node].right = insertAvl(nodes[node].right, key);
    else return node;
    return rebalance(node);
}

// Добавляем лист и возвращаем его индекс.
int SearchTree::addNode(int key) {
    if (!freeNodes.empty()) {
        int index = freeNodes.back();
        freeNodes.pop_back();
        nodes[index] = Node{key};
        return index;
    }
    nodes.push_back({key});
    return static_cast<int>(nodes.size()) - 1;
}

// При двух потомках заменяем ключ минимумом справа. После удаления балансируем путь к корню.
int SearchTree::eraseAvl(int node, int key) {
    if (node == -1) return -1;
    if (key < nodes[node].key) {
        nodes[node].left = eraseAvl(nodes[node].left, key);
    } else if (key > nodes[node].key) {
        nodes[node].right = eraseAvl(nodes[node].right, key);
    } else {
        int left = nodes[node].left, right = nodes[node].right;
        if (left == -1 || right == -1) {
            freeNodes.push_back(node);
            return left == -1 ? right : left;
        }
        int successor = right;
        while (nodes[successor].left != -1) successor = nodes[successor].left;
        int replacement = nodes[successor].key;
        nodes[node].right = eraseAvl(right, replacement);
        nodes[node].key = replacement;
    }
    return rebalance(node);
}

// Простая вставка BST через цикл работает и на длинной цепочке.
void SearchTree::insertBst(int key) {
    if (root == -1) {
        root = addNode(key);
        return;
    }
    int current = root;
    while (key != nodes[current].key) {
        bool left = key < nodes[current].key;
        int next = left ? nodes[current].left : nodes[current].right;
        if (next == -1) {
            next = addNode(key);
            if (left) nodes[current].left = next;
            else nodes[current].right = next;
            return;
        }
        current = next;
    }
}

// Оба дерева используют одинаковое хранение, отличается только балансировка.
SearchTree::SearchTree(bool avl) : balanceTree(avl) {}

// Выбираем алгоритм вставки для выбранного типа дерева.
void SearchTree::insert(int key) {
    if (balanceTree) root = insertAvl(root, key);
    else insertBst(key);
}

// Ищем ключ сверху вниз по порядку BST.
bool SearchTree::contains(int key) const {
    int node = root;
    while (node != -1 && nodes[node].key != key) {
        node = key < nodes[node].key ? nodes[node].left : nodes[node].right;
    }
    return node != -1;
}

// Самый правый узел содержит максимум.
std::optional<int> SearchTree::maximum() const {
    if (root == -1) return std::nullopt;
    int node = root;
    while (nodes[node].right != -1) node = nodes[node].right;
    return nodes[node].key;
}

// Удаляем ключ из сбалансированного дерева и обновляем его корень.
void SearchTree::removeFromAvl(int key) {
    if (balanceTree) root = eraseAvl(root, key);
}

// Высота AVL доступна в корне.
int SearchTree::avlHeight() const { return height(root); }
