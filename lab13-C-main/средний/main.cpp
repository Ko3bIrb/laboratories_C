#include "Kruskal.h"

int main() {
    setupConsole();
    const auto& cities = exampleCities();
    const auto result = kruskal(static_cast<int>(cities.size()), exampleRoads());
    printForest("Алгоритм Краскала (DSU)", result, cities);
    return result.cost == 15 ? 0 : 1;
}
