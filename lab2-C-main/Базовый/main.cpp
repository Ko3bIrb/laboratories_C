#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

// Базовый уровень: анализ сложности

// O(n)
int exampleA(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return sum;
}

// O(n^2)
int exampleB(int arr[], int n) {
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (arr[i] == arr[j]) count++;
    return count;
}

// O(log n)
int exampleC(int n) {
    int count = 0;
    for (int i = 1; i < n; i *= 2) count++;
    return count;
}

// O(n log n)
int exampleD(int n) {
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = n; j > 0; j /= 2) count++;
    return count;
}

// O(sqrt(n))
int exampleE(int n) {
    int count = 0;
    for (int i = 0; i * i < n; i++) count++;
    return count;
}

// Базовый уровень: шаблонная функция (вариант 2)
template <typename T>
T myMin(T a, T b) {
    return (a < b) ? a : b;
}

// Средний уровень: шаблонный класс Triple (вариант 2)
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

// Средний уровень: сравнение двух реализаций поиска
template <typename T>
int linearSearch(const std::vector<T>& arr, const T& value) {
    for (size_t i = 0; i < arr.size(); i++)
        if (arr[i] == value) return (int)i;
    return -1;
}

template <typename T>
int binarySearch(const std::vector<T>& arr, const T& value) {
    int left = 0, right = (int)arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == value) return mid;
        if (arr[mid] < value) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

template <typename Func>
long long measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    // myMin
    std::cout << "myMin(int): " << myMin(5, 10) << std::endl;
    std::cout << "myMin(double): " << myMin(3.14, 2.71) << std::endl;
    std::cout << "myMin(string): " << myMin(std::string("apple"), std::string("banana")) << std::endl;
    std::cout << "myMin(char): " << myMin('z', 'a') << std::endl;

    // Triple
    Triple<int, double, std::string> t(42, 3.14, "hello");
    std::cout << "\nTriple: " << t.getFirst() << ", "
        << t.getSecond() << ", " << t.getThird() << std::endl;
    t.setFirst(100);
    std::cout << "After setFirst: " << t.getFirst() << std::endl;

    // Сравнение поиска
    std::vector<int> sizes = { 10000, 100000, 500000, 1000000 };
    std::cout << "\nSize\tLinear(us)\tBinary(us)" << std::endl;
    for (int n : sizes) {
        std::vector<int> arr(n);
        for (int i = 0; i < n; i++) arr[i] = i;
        int target = n - 1;

        long long lt = measureTime([&]() { linearSearch(arr, target); });
        long long bt = measureTime([&]() { binarySearch(arr, target); });

        std::cout << n << "\t" << lt << "\t\t" << bt << std::endl;
    }

    return 0;
}