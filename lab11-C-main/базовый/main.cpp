#include "Graph.h"

int main() {
    setupConsole();
    auto graph = exampleGraph();
    graph.setEdge(0, 6, true);
    graph.setEdge(0, 6, false);
    std::cout << "Матрица после добавления и удаления ребра 0–6:\n";
    graph.print();
    printVertices("Рекурсивный DFS из 0: ", graph.dfs(0));
    printVertices("DFS со стеком из 0: ", graph.dfsIterative(0));
    printVertices("Достижимые из 0 (BFS): ", graph.bfs(0));
}
