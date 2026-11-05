#ifndef TRIPLE_H
#define TRIPLE_H

#include <vector>
#include <chrono>

// Объявления функций анализа сложности
int exampleA(int arr[], int n); // O(n)
int exampleB(int arr[], int n); // O(n^2)
int exampleC(int n);            // O(log n)
int exampleD(int n);            // O(n log n)
int exampleE(int n);            // O(sqrt(n))

// Шаблонная функция
template <typename T>
T myMin(T a, T b) {
    return (a < b) ? a : b;   // O(1)
}

// Шаблонный класс
template <typename T1, typename T2, typename T3>
class Triple {
private:
    T1 first;
    T2 second;
    T3 third;
public:
    Triple(T1 a, T2 b, T3 c) : first(a), second(b), third(c) {} // O(1)
    T1 getFirst()  const { return first; }  // O(1)
    T2 getSecond() const { return second; }  // O(1)
    T3 getThird()  const { return third; }  // O(1)
    void setFirst(T1 v) { first = v; }     // O(1)
    void setSecond(T2 v) { second = v; }     // O(1)
    void setThird(T3 v) { third = v; }     // O(1)
};

// Поиск
template <typename T>
int linearSearch(const std::vector<T>& arr, const T& value) { // O(n)
    for (size_t i = 0; i < arr.size(); i++)
        if (arr[i] == value) return (int)i;
    return -1;
}

template <typename T>
int binarySearch(const std::vector<T>& arr, const T& value) { // O(log n)
    int left = 0, right = (int)arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == value) return mid;
        if (arr[mid] < value) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// Замер времени
template <typename Func>
long long measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

#endif