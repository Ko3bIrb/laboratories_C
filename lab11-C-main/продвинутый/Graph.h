#pragma once
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

int runGraphApp();

class Graph {
    std::vector<std::vector<bool>> matrix;

    void visit(int vertex, std::vector<bool>& seen, std::vector<int>& order) const;

    std::vector<int> parentsFrom(int start, std::vector<int>& order) const;

public:
    explicit Graph(int vertices);

    int size() const;

    void setEdge(int from, int to, bool present);

    void print() const;

    std::vector<int> dfs(int start) const;

    std::vector<int> dfsIterative(int start) const;

    std::vector<int> bfs(int start) const;

    std::vector<std::vector<int>> components() const;

    std::vector<int> shortestPath(int start, int finish) const;
};

Graph exampleGraph();
void printVertices(const char* label, const std::vector<int>& vertices);

void setupConsole();
