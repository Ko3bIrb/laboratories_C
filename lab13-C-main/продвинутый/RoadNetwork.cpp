#include "RoadNetwork.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <queue>
#include <random>
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

// Сравниваем алгоритмы на одних графах с цепочкой и дополнительными дорогами.
static void benchmark() {
    std::mt19937 random(13);
    std::uniform_int_distribution<int> cost(1, 100);
    using Us = std::chrono::duration<double, std::micro>;
    std::cout << "\nГорода | Дороги | Прим, мкс | Краскал, мкс | Стоимость\n";
    for (int size : {50, 200, 500}) {
        std::vector<Road> roads;
        for (int from = 0; from < size; ++from) {
            for (int to = from + 1; to < size; ++to) {
                if (to == from + 1 || random() % 20 == 0) roads.push_back({from, to, cost(random)});
            }
        }
        auto start = std::chrono::steady_clock::now();
        Forest a = prim(size, roads);
        auto middle = std::chrono::steady_clock::now();
        Forest b = kruskal(size, roads);
        auto end = std::chrono::steady_clock::now();
        if (a.cost != b.cost) throw std::logic_error("Стоимость остовов различается");
        std::cout << size << " | " << roads.size() << " | " << Us(middle - start).count()
                  << " | " << Us(end - middle).count() << " | " << a.cost << '\n';
    }
}

// Принимаем свой список дорог или используем пример из методички.
int runNetwork() {
    std::cout << "0 — пример, 1 — свои дороги: ";
    int mode;
    if (!(std::cin >> mode) || (mode != 0 && mode != 1)) return 1;
    auto names = exampleCities();
    auto roads = exampleRoads();
    if (mode == 1) {
        int cities, count;
        std::cout << "Число городов и дорог: ";
        if (!(std::cin >> cities >> count) || cities < 1 || cities > 10000 || count < 0) return 1;
        names.resize(cities);
        for (int i = 0; i < cities; ++i) names[i] = std::to_string(i);
        roads.clear();
        std::cout << "Для каждой дороги введите два номера города (от 0) и стоимость.\n";
        for (int i = 0; i < count; ++i) {
            Road road{};
            if (!(std::cin >> road.from >> road.to >> road.cost)
                || road.from < 0 || road.to < 0 || road.from >= cities || road.to >= cities
                || road.from == road.to || road.cost < 0) return 1;
            roads.push_back(road);
        }
    }
    int size = static_cast<int>(names.size());
    auto a = prim(size, roads), b = kruskal(size, roads);
    printForest("Алгоритм Прима", a, names);
    printForest("Алгоритм Краскала", b, names);
    if (a.cost != b.cost) return 1;
    if (mode == 0) benchmark();
    return 0;
}

// Добавляем к каждой компоненте самое дешёвое ребро, ведущее в новый город.
Forest prim(int size, const std::vector<Road>& roads) {
    std::vector<std::vector<Road>> adjacent(size);
    for (Road road : roads) {
        adjacent[road.from].push_back(road);
        adjacent[road.to].push_back({road.to, road.from, road.cost});
    }
    auto moreExpensive = [](Road a, Road b) { return cheaper(b, a); };
    std::priority_queue<Road, std::vector<Road>, decltype(moreExpensive)> queue(moreExpensive);
    Forest result;
    std::vector<bool> used(size);
    for (int start = 0; start < size; ++start) {
        if (used[start]) continue;
        result.groups.emplace_back();
        queue.push({-1, start, 0});
        while (!queue.empty()) {
            Road road = queue.top();
            queue.pop();
            // Если оба конца уже в остове, добавление дороги создало бы цикл.
            if (used[road.to]) continue;
            used[road.to] = true;
            result.groups.back().push_back(road.to);
            if (road.from != -1) result.add(road);
            for (Road next : adjacent[road.to]) {
                if (!used[next.to]) queue.push(next);
            }
        }
    }
    return result;
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
