#pragma once
#include <memory>

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

void setupConsole();
