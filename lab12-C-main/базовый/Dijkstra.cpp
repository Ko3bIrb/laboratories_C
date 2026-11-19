#include "Dijkstra.h"
#include <functional>
#include <queue>
#include <stdexcept>
#include <utility>

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

// Поднимаемся от цели до начала и разворачиваем найденный путь.
std::vector<int> FromStart::path(int to) const {
    if (parent[to] == -1) return {};
    std::vector<int> result;
    while (parent[to] != to) {
        result.push_back(to);
        to = parent[to];
    }
    result.push_back(to);
    std::reverse(result.begin(), result.end());
    return result;
}

// Для каждого улучшения кладём новую пару в очередь, старую пропускаем при извлечении.
FromStart dijkstra(const Graph& graph, int start) {
    using Item = std::pair<long long, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> queue;
    FromStart result{Distances(graph.size(), infinity), std::vector<int>(graph.size(), -1)};
    auto& distance = result.distance;
    distance[start] = 0;
    result.parent[start] = start;
    queue.push({0, start});
    while (!queue.empty()) {
        auto [length, from] = queue.top();
        queue.pop();
        // При улучшении в куче остаётся старая оценка. Пропускаем её.
        if (length != distance[from]) continue;
        for (auto edge : graph[from]) {
            if (edge.weight < 0) throw std::invalid_argument("Дейкстре нужны неотрицательные веса");
            long long candidate = length + edge.weight;
            if (candidate >= distance[edge.to]) continue;
            distance[edge.to] = candidate;
            result.parent[edge.to] = from;
            queue.push({candidate, edge.to});
        }
    }
    return result;
}
