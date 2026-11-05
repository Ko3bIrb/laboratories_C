#include "Structures.h"

#include <iostream>
#include <sstream>
#include <string>

int evaluateRpn(const std::string& expression) {
    Stack<int> values;
    std::istringstream input(expression);
    std::string token;
    while (input >> token) {
        if (token == "+" || token == "-" || token == "*") {
            int right = values.pop();
            int left = values.pop();
            if (token == "+") values.push(left + right);
            if (token == "-") values.push(left - right);
            if (token == "*") values.push(left * right);
        } else {
            values.push(std::stoi(token));
        }
    }
    return values.pop();
}

int main() {
    std::cout << "Средний уровень, вариант 2\n";
    std::cout << "RPN 3 4 + 5 * = " << evaluateRpn("3 4 + 5 *") << '\n';

    QueueTwoStacks<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);
    std::cout << "Очередь на двух стеках: " << queue.dequeue()
              << ", затем " << queue.dequeue() << '\n';

    FixedDeque<int> deque(3);
    deque.push_back(2);
    deque.push_front(1);
    deque.push_back(3);
    std::cout << "Фиксированный дек: " << deque.pop_front()
              << " и " << deque.pop_back() << '\n';

    int numbers[] = {8, 3, 10, 1, 6, 4};
    std::vector<int> result = kSmallest(numbers, 6, 2);
    std::cout << "Два наименьших элемента: [" << result[0] << ", " << result[1] << "]\n";
    return 0;
}
