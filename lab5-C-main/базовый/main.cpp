#include "Structures.h"

#include <iostream>

int main() {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    std::cout << "Базовый уровень\nСтек: top = " << stack.top()
              << ", pop = " << stack.pop() << '\n';

    QueueTwoStacks<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    std::cout << "Очередь: front = " << queue.front()
              << ", dequeue = " << queue.dequeue() << '\n';

    FixedDeque<int> deque(3);
    deque.push_front(2);
    deque.push_back(3);
    deque.push_front(1);
    std::cout << "Дек: front = " << deque.front()
              << ", back = " << deque.back() << '\n';

    PriorityQueue<int> priorities;
    priorities.push(4);
    priorities.push(9);
    priorities.push(1);
    std::cout << "Приоритетная очередь: top = " << priorities.top() << '\n';
    return 0;
}
