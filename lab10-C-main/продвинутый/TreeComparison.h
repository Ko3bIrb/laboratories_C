#pragma once
#include <algorithm>
#include <optional>
#include <vector>

void compareTrees();

// Храним узлы в векторе и обращаемся к ним по индексам, чтобы не зависеть от адресов.
class SearchTree {
    struct Node { int key; int left = -1; int right = -1; int height = 1; };
    std::vector<Node> nodes;
    std::vector<int> freeNodes;
    int root = -1;
    bool balanceTree;

    int height(int node) const;

    void updateHeight(int node);

    int balance(int node) const;

    int rotateLeft(int node);

    int rotateRight(int node);

    int rebalance(int node);

    int insertAvl(int node, int key);

    int addNode(int key);

    int eraseAvl(int node, int key);

    void insertBst(int key);

public:
    explicit SearchTree(bool avl);

    void insert(int key);

    bool contains(int key) const;

    std::optional<int> maximum() const;

    void removeFromAvl(int key);

    int avlHeight() const;
};

void setupConsole();
