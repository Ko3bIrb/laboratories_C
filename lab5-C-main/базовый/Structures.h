#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
class Stack {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void reserve(std::size_t newCapacity) {
        if (newCapacity <= capacity_) return;
        T* newData = new T[newCapacity];
        for (std::size_t i = 0; i < size_; ++i) newData[i] = data_[i];
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

public:
    Stack() : data_(nullptr), size_(0), capacity_(0) {}

    Stack(const Stack& other) : data_(nullptr), size_(0), capacity_(0) {
        reserve(other.capacity_);
        for (std::size_t i = 0; i < other.size_; ++i) data_[i] = other.data_[i];
        size_ = other.size_;
    }

    Stack(Stack&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~Stack() { delete[] data_; }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            Stack copy(other);
            swap(copy);
        }
        return *this;
    }

    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    void push(const T& value) {
        if (size_ == capacity_) reserve(capacity_ == 0 ? 4 : capacity_ * 2);
        data_[size_++] = value;
    }

    T pop() {
        if (empty()) throw std::underflow_error("Stack is empty");
        return data_[--size_];
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("Stack is empty");
        return data_[size_ - 1];
    }

    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }

    void swap(Stack& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
};

// Очередь на двух стеках: входной и выходной стеки меняются ролями.
template <typename T>
class QueueTwoStacks {
private:
    Stack<T> input_;
    Stack<T> output_;

    void moveToOutput() {
        if (output_.empty()) {
            while (!input_.empty()) output_.push(input_.pop());
        }
    }

public:
    void enqueue(const T& value) { input_.push(value); }

    T dequeue() {
        moveToOutput();
        if (output_.empty()) throw std::underflow_error("Queue is empty");
        return output_.pop();
    }

    const T& front() {
        moveToOutput();
        if (output_.empty()) throw std::underflow_error("Queue is empty");
        return output_.top();
    }

    bool empty() const { return input_.empty() && output_.empty(); }
    std::size_t size() const { return input_.size() + output_.size(); }
};

template <typename T>
class FixedDeque {
private:
    T* data_;
    std::size_t capacity_;
    std::size_t frontIndex_;
    std::size_t size_;

public:
    explicit FixedDeque(std::size_t capacity)
        : data_(capacity == 0 ? nullptr : new T[capacity]),
          capacity_(capacity), frontIndex_(0), size_(0) {
        if (capacity == 0) throw std::invalid_argument("Deque capacity must be positive");
    }

    ~FixedDeque() { delete[] data_; }

    FixedDeque(const FixedDeque& other)
        : data_(new T[other.capacity_]), capacity_(other.capacity_),
          frontIndex_(other.frontIndex_), size_(other.size_) {
        for (std::size_t i = 0; i < capacity_; ++i) data_[i] = other.data_[i];
    }

    FixedDeque& operator=(const FixedDeque& other) {
        if (this != &other) {
            FixedDeque copy(other);
            swap(copy);
        }
        return *this;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) throw std::overflow_error("Deque is full");
        data_[(frontIndex_ + size_) % capacity_] = value;
        ++size_;
    }

    void push_front(const T& value) {
        if (size_ == capacity_) throw std::overflow_error("Deque is full");
        frontIndex_ = (frontIndex_ + capacity_ - 1) % capacity_;
        data_[frontIndex_] = value;
        ++size_;
    }

    T pop_front() {
        if (empty()) throw std::underflow_error("Deque is empty");
        T value = data_[frontIndex_];
        frontIndex_ = (frontIndex_ + 1) % capacity_;
        --size_;
        return value;
    }

    T pop_back() {
        if (empty()) throw std::underflow_error("Deque is empty");
        std::size_t index = (frontIndex_ + size_ - 1) % capacity_;
        T value = data_[index];
        --size_;
        return value;
    }

    const T& front() const {
        if (empty()) throw std::underflow_error("Deque is empty");
        return data_[frontIndex_];
    }

    const T& back() const {
        if (empty()) throw std::underflow_error("Deque is empty");
        return data_[(frontIndex_ + size_ - 1) % capacity_];
    }

    bool empty() const { return size_ == 0; }
    bool full() const { return size_ == capacity_; }
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }

    void swap(FixedDeque& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(capacity_, other.capacity_);
        std::swap(frontIndex_, other.frontIndex_);
        std::swap(size_, other.size_);
    }
};

template <typename T>
class PriorityQueue {
private:
    T* heap_;
    std::size_t size_;
    std::size_t capacity_;

    void reserve(std::size_t newCapacity) {
        if (newCapacity <= capacity_) return;
        T* newHeap = new T[newCapacity];
        for (std::size_t i = 0; i < size_; ++i) newHeap[i] = heap_[i];
        delete[] heap_;
        heap_ = newHeap;
        capacity_ = newCapacity;
    }

    void moveUp(std::size_t index) {
        while (index > 0) {
            std::size_t parent = (index - 1) / 2;
            if (heap_[parent] >= heap_[index]) break;
            std::swap(heap_[parent], heap_[index]);
            index = parent;
        }
    }

    void moveDown(std::size_t index) {
        while (true) {
            std::size_t left = index * 2 + 1;
            std::size_t right = index * 2 + 2;
            std::size_t largest = index;
            if (left < size_ && heap_[left] > heap_[largest]) largest = left;
            if (right < size_ && heap_[right] > heap_[largest]) largest = right;
            if (largest == index) break;
            std::swap(heap_[index], heap_[largest]);
            index = largest;
        }
    }

public:
    PriorityQueue() : heap_(nullptr), size_(0), capacity_(0) {}
    ~PriorityQueue() { delete[] heap_; }

    void push(const T& value) {
        if (size_ == capacity_) reserve(capacity_ == 0 ? 4 : capacity_ * 2);
        heap_[size_] = value;
        moveUp(size_);
        ++size_;
    }

    T pop() {
        if (empty()) throw std::underflow_error("Priority queue is empty");
        T value = heap_[0];
        --size_;
        if (size_ > 0) {
            heap_[0] = heap_[size_];
            moveDown(0);
        }
        return value;
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("Priority queue is empty");
        return heap_[0];
    }

    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }
};

template <typename T>
std::vector<T> kSmallest(const T* values, std::size_t count, std::size_t k) {
    if (k > count) k = count;
    PriorityQueue<T> largest;
    for (std::size_t i = 0; i < count; ++i) {
        if (largest.size() < k) {
            largest.push(values[i]);
        } else if (k > 0 && values[i] < largest.top()) {
            largest.pop();
            largest.push(values[i]);
        }
    }

    std::vector<T> result;
    while (!largest.empty()) result.push_back(largest.pop());
    std::sort(result.begin(), result.end());
    return result;
}

#endif
