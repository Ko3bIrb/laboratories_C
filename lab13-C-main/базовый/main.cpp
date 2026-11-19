#include "Prim.h"

int main() {
    setupConsole();
    const auto& cities = exampleCities();
    const auto result = prim(static_cast<int>(cities.size()), exampleRoads());
    printForest("Алгоритм Прима", result, cities);
    return result.cost == 15 ? 0 : 1;
}
