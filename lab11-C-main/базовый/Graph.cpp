#include "Graph.h"
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

// Отмечаем вершину до рекурсии, чтобы не ходить по кругу.
void Graph::visit(int vertex, std::vector<bool>& seen, std::vector<int>& order) const {
    seen[vertex] = true;
    order.push_back(vertex);
    for (int neighbor = 0; neighbor < size(); ++neighbor) {
        if (matrix[vertex][neighbor] && !seen[neighbor]) visit(neighbor, seen, order);
    }
}

// BFS запоминает, из какой вершины впервые пришёл в каждую соседнюю.
std::vector<int> Graph::parentsFrom(int start, std::vector<int>& order) const {
    std::vector<int> parent(size(), -1);
    std::queue<int> pending;
    // Значение −1 значит «не посещена». У старта предок равен самому старту.
    parent[start] = start;
    pending.push(start);
    while (!pending.empty()) {
        int vertex = pending.front();
        pending.pop();
        order.push_back(vertex);
        for (int next = 0; next < size(); ++next) {
            if (!matrix[vertex][next] || parent[next] != -1) continue;
            // Отмечаем при добавлении в очередь, чтобы добавить соседа только один раз.
            parent[next] = vertex;
            pending.push(next);
        }
    }
    return parent;
}

// У нового графа пока нет рёбер.
Graph::Graph(int vertices) : matrix(vertices, std::vector<bool>(vertices)) {}

// Количество строк равно количеству вершин.
int Graph::size() const { return static_cast<int>(matrix.size()); }

// Одно неориентированное ребро занимает две симметричные клетки.
void Graph::setEdge(int from, int to, bool present) {
    matrix.at(from).at(to) = present;
    matrix.at(to).at(from) = present;
}

// Печатаем матрицу и нумерацию вершин.
void Graph::print() const {
    std::cout << "   ";
    for (int i = 0; i < size(); ++i) std::cout << i << ' ';
    std::cout << '\n';
    for (int i = 0; i < size(); ++i) {
        std::cout << i << ": ";
        for (bool edge : matrix[i]) std::cout << edge << ' ';
        std::cout << '\n';
    }
}

// Рекурсивный DFS возвращает все вершины, достижимые из start.
std::vector<int> Graph::dfs(int start) const {
    std::vector<int> order;
    std::vector<bool> seen(size());
    visit(start, seen, order);
    return order;
}

// В итеративном DFS стек хранит вершины, которые ещё предстоит обработать.
std::vector<int> Graph::dfsIterative(int start) const {
    std::vector<int> order;
    std::vector<bool> seen(size());
    std::stack<int> pending;
    pending.push(start);
    while (!pending.empty()) {
        int vertex = pending.top();
        pending.pop();
        if (seen[vertex]) continue;
        seen[vertex] = true;
        order.push_back(vertex);
        // Кладём большие номера раньше: из стека первым выйдет меньший.
        for (int next = size() - 1; next >= 0; --next) {
            if (matrix[vertex][next] && !seen[next]) pending.push(next);
        }
    }
    return order;
}

// BFS посещает вершины от ближних к дальним.
std::vector<int> Graph::bfs(int start) const {
    std::vector<int> order;
    parentsFrom(start, order);
    return order;
}

// Один DFS обходит целую компоненту. Следующий начинаем с непосещённой вершины.
std::vector<std::vector<int>> Graph::components() const {
    std::vector<std::vector<int>> groups;
    std::vector<bool> seen(size());
    for (int i = 0; i < size(); ++i) {
        if (seen[i]) continue;
        groups.emplace_back();
        visit(i, seen, groups.back());
    }
    return groups;
}

// Идём по родителям от конца к началу, потом разворачиваем путь.
std::vector<int> Graph::shortestPath(int start, int finish) const {
    std::vector<int> order, path;
    auto parent = parentsFrom(start, order);
    if (parent[finish] == -1) return path;
    for (int vertex = finish; vertex != start; vertex = parent[vertex]) path.push_back(vertex);
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

// Две компоненты: цикл 0–6 и отдельное ребро 7–8.
Graph exampleGraph() {
    Graph graph(9);
    for (auto edge : {std::pair{0,1}, {1,2}, {2,3}, {3,6}, {6,4}, {4,5}, {5,0}, {7,8}}) {
        graph.setEdge(edge.first, edge.second, true);
    }
    return graph;
}

// Печатаем вершины пути или обхода в одну строку.
void printVertices(const char* label, const std::vector<int>& vertices) {
    std::cout << label;
    for (int vertex : vertices) std::cout << vertex << ' ';
    std::cout << '\n';
}
