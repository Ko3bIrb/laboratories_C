#pragma once
#include <vector>

void showDirectedExample();

constexpr long long infinity = 1'000'000'000'000'000LL;
using Distances = std::vector<long long>;
struct Edge { int to; int weight; };
using Graph = std::vector<std::vector<Edge>>;

struct Paths {
    std::vector<Distances> distance;
    std::vector<std::vector<int>> next;

    std::vector<int> path(int from, int to) const;
};

Paths floydWarshall(const Graph& graph);

void setupConsole();
