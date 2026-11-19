#include "Prim.h"
#include <iostream>
#include <queue>

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
