#pragma once
#include <map>
#include <memory>
#include <string_view>

class Trie {
    struct Node {
        std::map<char, std::unique_ptr<Node>> children;
        bool word = false;
    };
    Node root;

    static bool erase(Node& node, std::string_view suffix);

public:
    void insert(std::string_view word);

    bool contains(std::string_view word) const;

    bool remove(std::string_view word);
};

void setupConsole();
