#pragma once
#include <string>
#include <tuple>
#include <vector>

struct Road { int from, to, cost; };
struct Forest {
    std::vector<Road> roads;
    std::vector<std::vector<int>> groups;
    long long cost = 0;

    void add(Road road);
};

// Сравниваем сначала стоимость, затем номера городов для воспроизводимого вывода.
inline bool cheaper(Road a, Road b) {
    return std::tie(a.cost, a.from, a.to) < std::tie(b.cost, b.from, b.to);
}

const std::vector<std::string>& exampleCities();
const std::vector<Road>& exampleRoads();
void printForest(const char* algorithm, const Forest& forest, const std::vector<std::string>& names);

Forest prim(int size, const std::vector<Road>& roads);

void setupConsole();
