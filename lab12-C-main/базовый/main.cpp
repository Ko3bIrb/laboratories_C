#include "Dijkstra.h"
#include <iostream>

int main() {
    setupConsole();
    // Рёбра симметричны. Последняя вершина изолирована.
    const Graph graph{{{1, 4}, {2, 1}}, {{0, 4}, {2, 2}, {3, 1}},
                      {{0, 1}, {1, 2}, {3, 6}}, {{1, 1}, {2, 6}}, {}};
    auto result = dijkstra(graph, 0);
    std::cout << "Расстояния из 0:";
    for (auto distance : result.distance) {
        if (distance == infinity) std::cout << " INF";
        else std::cout << ' ' << distance;
    }
    std::cout << "\nПуть до 3:";
    for (int vertex : result.path(3)) std::cout << ' ' << vertex;
    std::cout << '\n';
}
