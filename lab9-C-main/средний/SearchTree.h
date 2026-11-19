#pragma once
#include <memory>
#include <optional>
#include <utility>

// unique_ptr освобождает каждый узел вместе с его поддеревьями.
struct Node {
    int value;
    std::unique_ptr<Node> left, right;
    explicit Node(int key);
};

void preOrder(const Node* node);
void inOrder(const Node* node);
void postOrder(const Node* node);
void levelOrder(const Node* node);
int countLeaves(const Node* node);
int height(const Node* node);
void printTree(const Node* node, int depth = 0);
class BinaryTree {
public:
    std::unique_ptr<Node> root;
};

class SearchTree : public BinaryTree {
    static std::optional<int> kth(const Node* node, int& remaining);

    static bool erase(std::unique_ptr<Node>& node, int key);

public:
    void insert(int key);

    bool search(int key) const;

    std::optional<int> kthSmallest(int k) const;

    std::optional<int> extreme(bool maximum) const;

    bool remove(int key);
};

void setupConsole();
