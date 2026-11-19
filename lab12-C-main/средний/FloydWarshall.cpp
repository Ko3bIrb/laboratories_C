#include "FloydWarshall.h"
#include <iomanip>
#include <iostream>
#include <stdexcept>

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

// Идём по записанным первым шагам до нужной вершины.
std::vector<int> Paths::path(int from, int to) const {
    if (next[from][to] == -1) return {};
    std::vector<int> result{from};
    while (from != to) {
        from = next[from][to];
        result.push_back(from);
    }
    return result;
}

// Сначала есть только прямые рёбра. Затем разрешаем новые промежуточные вершины.
Paths floydWarshall(const Graph& graph) {
    int size = static_cast<int>(graph.size());
    Paths result{std::vector<Distances>(size, Distances(size, infinity)),
                 std::vector<std::vector<int>>(size, std::vector<int>(size, -1))};
    auto& distance = result.distance;
    for (int from = 0; from < size; ++from) {
        distance[from][from] = 0;
        result.next[from][from] = from;
        for (auto edge : graph[from]) {
            if (edge.weight < distance[from][edge.to]) {
                distance[from][edge.to] = edge.weight;
                result.next[from][edge.to] = edge.to;
            }
        }
    }
    // Внешний цикл по middle меняет набор вершин, через которые уже можно строить путь.
    for (int middle = 0; middle < size; ++middle) {
        for (int from = 0; from < size; ++from) {
            if (distance[from][middle] == infinity) continue;
            for (int to = 0; to < size; ++to) {
                if (distance[middle][to] == infinity) continue;
                long long candidate = distance[from][middle] + distance[middle][to];
                if (candidate >= distance[from][to]) continue;
                distance[from][to] = candidate;
                // Для нового маршрута первый шаг совпадает с первым шагом на пути к middle.
                result.next[from][to] = result.next[from][middle];
            }
        }
    }
    // При отрицательном цикле кратчайший путь может не иметь конечного веса.
    for (int vertex = 0; vertex < size; ++vertex) {
        if (distance[vertex][vertex] < 0) throw std::logic_error("Обнаружен отрицательный цикл");
    }
    return result;
}

// Печатаем бесконечные расстояния как INF, чтобы матрица читалась легче.
static void printMatrix(const Paths& paths) {
    for (const auto& row : paths.distance) {
        for (auto value : row) {
            if (value == infinity) std::cout << std::setw(5) << "INF";
            else std::cout << std::setw(5) << value;
        }
        std::cout << '\n';
    }
}

void showDirectedExample() {
    Graph directed{{{1, 3}, {2, 14}}, {{2, -2}}, {{3, 4}}, {}};
    Paths paths = floydWarshall(directed);
    std::cout << "Матрица ориентированного графа:\n";
    printMatrix(paths);
    std::cout << "Путь 0 -> 3: ";
    for (int vertex : paths.path(0, 3)) std::cout << vertex << ' ';
    std::cout << '\n';
}
