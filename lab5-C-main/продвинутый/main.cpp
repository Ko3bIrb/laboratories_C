#include "Structures.h"

#include <iostream>
#include <sstream>
#include <string>

int evaluateRpn(const std::string& expression) {
    Stack<int> values;
    std::istringstream input(expression);
    std::string token;

    while (input >> token) {
        if (token.size() == 1 && std::string("+-*/").find(token[0]) != std::string::npos) {
            int right = values.pop();
            int left = values.pop();
            if (token[0] == '+') values.push(left + right);
            if (token[0] == '-') values.push(left - right);
            if (token[0] == '*') values.push(left * right);
            if (token[0] == '/') {
                if (right == 0) throw std::invalid_argument("Division by zero");
                values.push(left / right);
            }
        } else {
            values.push(std::stoi(token));
        }
    }

    int answer = values.pop();
    if (!values.empty()) throw std::invalid_argument("Invalid RPN expression");
    return answer;
}

template <typename T>
void printValues(const std::vector<T>& values) {
    std::cout << "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) std::cout << ", ";
        std::cout << values[i];
    }
    std::cout << "]";
}

int main() {
    std::cout << "Стек: обратная польская запись\n";
    std::cout << "3 4 + 5 * = " << evaluateRpn("3 4 + 5 *") << '\n';

    std::cout << "\nОчередь на двух стеках:\n";
    QueueTwoStacks<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);
    std::cout << "front = " << queue.front() << ", dequeue = " << queue.dequeue() << '\n';
    std::cout << "следующий элемент = " << queue.front() << '\n';

    std::cout << "\nДек с фиксированной ёмкостью 4:\n";
    FixedDeque<int> deque(4);
    deque.push_back(2);
    deque.push_back(3);
    deque.push_front(1);
    deque.push_front(0);
    std::cout << "front = " << deque.front() << ", back = " << deque.back()
              << ", size = " << deque.size() << '\n';
    std::cout << "pop_front = " << deque.pop_front()
              << ", pop_back = " << deque.pop_back() << '\n';

    std::cout << "\nПриоритетная очередь (max-heap): ";
    PriorityQueue<int> priorities;
    priorities.push(30);
    priorities.push(10);
    priorities.push(50);
    priorities.push(20);
    while (!priorities.empty()) std::cout << priorities.pop() << ' ';
    std::cout << '\n';

    int numbers[] = {8, 3, 10, 1, 6, 4, 9, 2};
    std::vector<int> smallest = kSmallest(numbers, 8, 3);
    std::cout << "Три наименьших элемента: ";
    printValues(smallest);
    std::cout << '\n';

    try {
        FixedDeque<int> fullDeque(2);
        fullDeque.push_back(1);
        fullDeque.push_back(2);
        fullDeque.push_back(3);
    } catch (const std::overflow_error& error) {
        std::cout << "Проверка переполнения дека: " << error.what() << '\n';
    }

    return 0;
}
