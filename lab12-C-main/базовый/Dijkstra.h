#pragma once
#include <algorithm>
#include <vector>

constexpr long long infinity = 1'000'000'000'000'000LL;
using Distances = std::vector<long long>;
struct Edge { int to; int weight; };
using Graph = std::vector<std::vector<Edge>>;

// Предки нужны для восстановления пути от одной начальной вершины.
struct FromStart {
    Distances distance;
    std::vector<int> parent;

    std::vector<int> path(int to) const;
};

FromStart dijkstra(const Graph& graph, int start);

void setupConsole();
