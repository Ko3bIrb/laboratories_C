#include "Kruskal.h"
#include <algorithm>
#include <iostream>

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

// Храним рёбра в порядке выбора алгоритмом.
void Forest::add(Road road) {
    roads.push_back(road);
    cost += road.cost;
}

// Каждый город изначально составляет своё собственное множество.
DisjointSets::DisjointSets(int size) : parent(size), rank(size) {
    std::iota(parent.begin(), parent.end(), 0);
}

// Во время поиска корня сокращаем путь для следующих запросов.
int DisjointSets::find(int vertex) {
    if (parent[vertex] != vertex) parent[vertex] = find(parent[vertex]);
    return parent[vertex];
}

// Объединяем меньшую по рангу структуру с большей. Общее множество означало бы цикл.
bool DisjointSets::join(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return false;
    if (rank[a] < rank[b]) std::swap(a, b);
    parent[b] = a;
    // Оценка высоты увеличивается только при объединении деревьев одинакового ранга.
    if (rank[a] == rank[b]) ++rank[a];
    return true;
}

// Сортируем рёбра по цене и берём те, которые соединяют разные множества.
Forest kruskal(int size, std::vector<Road> roads) {
    std::sort(roads.begin(), roads.end(), cheaper);
    DisjointSets dsu(size);
    Forest result;
    for (Road road : roads) {
        if (dsu.join(road.from, road.to)) result.add(road);
    }
    std::vector<int> groupIndex(size, -1);
    for (int vertex = 0; vertex < size; ++vertex) {
        int root = dsu.find(vertex);
        if (groupIndex[root] == -1) {
            groupIndex[root] = static_cast<int>(result.groups.size());
            result.groups.emplace_back();
        }
        result.groups[groupIndex[root]].push_back(vertex);
    }
    return result;
}

// Один и тот же пример нужен для сопоставления стоимости, выбранной двумя алгоритмами.
const std::vector<std::string>& exampleCities() {
    static const std::vector<std::string> names{"A", "B", "C", "D", "E", "F"};
    return names;
}

const std::vector<Road>& exampleRoads() {
    static const std::vector<Road> roads{{0,1,4}, {0,2,3}, {1,2,2}, {1,3,2}, {2,4,3}, {3,4,5}, {4,5,5}};
    return roads;
}

// Выводим стоимость, порядок выбора дорог и компоненты несвязного графа.
void printForest(const char* algorithm, const Forest& forest, const std::vector<std::string>& names) {
    std::cout << algorithm << '\n';
    if (forest.groups.size() == 1) std::cout << "Минимальная стоимость: " << forest.cost << '\n';
    else std::cout << "Все города соединить нельзя. Стоимость леса: " << forest.cost << '\n';
    for (Road road : forest.roads) {
        std::cout << names[road.from] << " - " << names[road.to] << " (" << road.cost << ")\n";
    }
    if (forest.groups.size() > 1) {
        for (const auto& group : forest.groups) {
            std::cout << "Компонента:";
            for (int vertex : group) std::cout << ' ' << names[vertex];
            std::cout << '\n';
        }
    }
}
