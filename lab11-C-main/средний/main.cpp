#include "Graph.h"

int main() {
    setupConsole();
    auto graph = exampleGraph();
    auto groups = graph.components();
    std::cout << "Компонент связности: " << groups.size() << '\n';
    for (const auto& group : groups) printVertices("Компонента: ", group);
    printVertices("Достижимые из 7: ", graph.bfs(7));
}
