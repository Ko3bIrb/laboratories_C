#include "ShortestPaths.h"
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
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

// Случайные неотрицательные веса, около 20% возможных рёбер и связующая цепочка.
static Graph randomGraph(int size, std::mt19937& random) {
    Graph graph(size);
    std::uniform_int_distribution<int> percentage(0, 99), weight(1, 50);
    for (int from = 0; from < size; ++from) {
        for (int to = from + 1; to < size; ++to) {
            if (to != from + 1 && percentage(random) >= 20) continue;
            int w = weight(random);
            graph[from].push_back({to, w});
            graph[to].push_back({from, w});
        }
    }
    return graph;
}

// Обоим алгоритмам даём одинаковый граф. У Флойда берём строку того же старта.
bool comparePaths() {
    std::cout << "V | Дейкстра, мкс | Флойд–Уоршелл, мкс | Строки равны\n";
    std::mt19937 random(12);
    using Us = std::chrono::duration<double, std::micro>;
    for (int size : {10, 20, 50, 100}) {
        Graph graph = randomGraph(size, random);
        int start = std::uniform_int_distribution<int>(0, size - 1)(random);
        constexpr int repeat = 12;
        double single = 0, all = 0;
        bool equal = true;
        FromStart fromStart;
        for (int i = 0; i < repeat; ++i) {
            auto before = std::chrono::steady_clock::now();
            fromStart = dijkstra(graph, start);
            auto between = std::chrono::steady_clock::now();
            auto pairs = floydWarshall(graph);
            auto after = std::chrono::steady_clock::now();
            single += Us(between - before).count();
            all += Us(after - between).count();
            equal = equal && fromStart.distance == pairs.distance[start];
        }
        std::cout << size << " | " << single / repeat << " | " << all / repeat
                  << " | " << (equal ? "да" : "нет") << '\n';
        if (!equal) return false;
        if (size == 20) {
            std::cout << "Старт " << start << ", общие расстояния:";
            for (auto distance : fromStart.distance) std::cout << ' ' << distance;
            std::cout << "\nПуть Дейкстры до 19:";
            for (int vertex : fromStart.path(19)) std::cout << ' ' << vertex;
            std::cout << '\n';
        }
    }
    return true;
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
