#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T>
class SinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;

        explicit Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;

public:
    class Iterator {
    private:
        Node* node_;
        explicit Iterator(Node* node) : node_(node) {}
        friend class SinglyLinkedList;

    public:
        T& operator*() const { return node_->data; }
        Iterator& operator++() {
            if (node_ != nullptr) node_ = node_->next;
            return *this;
        }
        bool operator==(const Iterator& other) const { return node_ == other.node_; }
        bool operator!=(const Iterator& other) const { return !(*this == other); }
    };

    class ConstIterator {
    private:
        const Node* node_;
        explicit ConstIterator(const Node* node) : node_(node) {}
        friend class SinglyLinkedList;

    public:
        const T& operator*() const { return node_->data; }
        ConstIterator& operator++() {
            if (node_ != nullptr) node_ = node_->next;
            return *this;
        }
        bool operator==(const ConstIterator& other) const { return node_ == other.node_; }
        bool operator!=(const ConstIterator& other) const { return !(*this == other); }
    };

    SinglyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    SinglyLinkedList(const SinglyLinkedList& other) : SinglyLinkedList() {
        for (const T& value : other) push_back(value);
    }

    SinglyLinkedList(SinglyLinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    ~SinglyLinkedList() { clear(); }

    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        if (this != &other) {
            SinglyLinkedList copy(other);
            swap(copy);
        }
        return *this;
    }

    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    void push_front(const T& value) {
        Node* node = new Node(value);
        node->next = head_;
        head_ = node;
        if (tail_ == nullptr) tail_ = node;
        ++size_;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        if (tail_ == nullptr) {
            head_ = tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }
        ++size_;
    }

    void pop_front() {
        if (empty()) throw std::underflow_error("SinglyLinkedList is empty");
        Node* oldHead = head_;
        head_ = head_->next;
        delete oldHead;
        --size_;
        if (head_ == nullptr) tail_ = nullptr;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("SinglyLinkedList is empty");
        if (head_ == tail_) {
            delete head_;
            head_ = tail_ = nullptr;
        } else {
            Node* current = head_;
            while (current->next != tail_) current = current->next;
            delete tail_;
            tail_ = current;
            tail_->next = nullptr;
        }
        --size_;
    }

    Iterator find(const T& value) {
        for (Iterator it = begin(); it != end(); ++it) {
            if (*it == value) return it;
        }
        return end();
    }

    bool remove(const T& value) {
        if (empty()) return false;
        if (head_->data == value) {
            pop_front();
            return true;
        }
        Node* current = head_;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }
        if (current->next == nullptr) return false;
        Node* removed = current->next;
        current->next = removed->next;
        if (removed == tail_) tail_ = current;
        delete removed;
        --size_;
        return true;
    }

    void clear() noexcept {
        while (head_ != nullptr) {
            Node* removed = head_;
            head_ = head_->next;
            delete removed;
        }
        tail_ = nullptr;
        size_ = 0;
    }

    std::size_t size() const { return size_; }
    bool empty() const { return head_ == nullptr; }

    void print() const {
        std::cout << "[";
        bool first = true;
        for (ConstIterator it = begin(); it != end(); ++it) {
            if (!first) std::cout << ", ";
            std::cout << *it;
            first = false;
        }
        std::cout << "]\n";
    }

    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }
    ConstIterator begin() const { return ConstIterator(head_); }
    ConstIterator end() const { return ConstIterator(nullptr); }

    void swap(SinglyLinkedList& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

    bool operator==(const SinglyLinkedList& other) const {
        if (size_ != other.size_) return false;
        ConstIterator left = begin();
        ConstIterator right = other.begin();
        while (left != end()) {
            if (*left != *right) return false;
            ++left;
            ++right;
        }
        return true;
    }

    bool operator!=(const SinglyLinkedList& other) const { return !(*this == other); }
};

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        explicit Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;

public:
    class Iterator {
    private:
        Node* node_;
        explicit Iterator(Node* node) : node_(node) {}
        friend class DoublyLinkedList;

    public:
        T& operator*() const { return node_->data; }
        Iterator& operator++() {
            if (node_ != nullptr) node_ = node_->next;
            return *this;
        }
        Iterator& operator--() {
            if (node_ != nullptr) node_ = node_->prev;
            return *this;
        }
        bool operator==(const Iterator& other) const { return node_ == other.node_; }
        bool operator!=(const Iterator& other) const { return !(*this == other); }
    };

    class ConstIterator {
    private:
        const Node* node_;
        explicit ConstIterator(const Node* node) : node_(node) {}
        friend class DoublyLinkedList;

    public:
        const T& operator*() const { return node_->data; }
        ConstIterator& operator++() {
            if (node_ != nullptr) node_ = node_->next;
            return *this;
        }
        bool operator==(const ConstIterator& other) const { return node_ == other.node_; }
        bool operator!=(const ConstIterator& other) const { return !(*this == other); }
    };

    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    DoublyLinkedList(const DoublyLinkedList& other) : DoublyLinkedList() {
        for (const T& value : other) push_back(value);
    }

    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    ~DoublyLinkedList() { clear(); }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            DoublyLinkedList copy(other);
            swap(copy);
        }
        return *this;
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    void push_front(const T& value) {
        Node* node = new Node(value);
        node->next = head_;
        if (head_ != nullptr) head_->prev = node;
        else tail_ = node;
        head_ = node;
        ++size_;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        node->prev = tail_;
        if (tail_ != nullptr) tail_->next = node;
        else head_ = node;
        tail_ = node;
        ++size_;
    }

    void pop_front() {
        if (empty()) throw std::underflow_error("DoublyLinkedList is empty");
        Node* oldHead = head_;
        head_ = head_->next;
        if (head_ != nullptr) head_->prev = nullptr;
        else tail_ = nullptr;
        delete oldHead;
        --size_;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("DoublyLinkedList is empty");
        Node* oldTail = tail_;
        tail_ = tail_->prev;
        if (tail_ != nullptr) tail_->next = nullptr;
        else head_ = nullptr;
        delete oldTail;
        --size_;
    }

    Iterator find(const T& value) {
        for (Iterator it = begin(); it != end(); ++it) {
            if (*it == value) return it;
        }
        return end();
    }

    bool remove(const T& value) {
        Iterator it = find(value);
        if (it == end()) return false;
        erase(it);
        return true;
    }

    void insert_after(Iterator position, const T& value) {
        if (position.node_ == nullptr) {
            push_back(value);
            return;
        }
        Node* node = new Node(value);
        node->prev = position.node_;
        node->next = position.node_->next;
        if (position.node_->next != nullptr) position.node_->next->prev = node;
        else tail_ = node;
        position.node_->next = node;
        ++size_;
    }

    void erase(Iterator position) {
        if (position.node_ == nullptr) return;
        if (position.node_ == head_) {
            pop_front();
            return;
        }
        if (position.node_ == tail_) {
            pop_back();
            return;
        }
        position.node_->prev->next = position.node_->next;
        position.node_->next->prev = position.node_->prev;
        delete position.node_;
        --size_;
    }

    // Слияние двух уже отсортированных списков по возрастанию.
    void merge(DoublyLinkedList& other) {
        DoublyLinkedList result;
        const DoublyLinkedList& thisList = *this;
        const DoublyLinkedList& otherList = other;
        ConstIterator left = thisList.begin();
        ConstIterator right = otherList.begin();
        while (left != thisList.end() && right != otherList.end()) {
            if (*left <= *right) {
                result.push_back(*left);
                ++left;
            } else {
                result.push_back(*right);
                ++right;
            }
        }
        while (left != thisList.end()) {
            result.push_back(*left);
            ++left;
        }
        while (right != otherList.end()) {
            result.push_back(*right);
            ++right;
        }
        *this = std::move(result);
        other.clear();
    }

    void clear() noexcept {
        while (head_ != nullptr) {
            Node* removed = head_;
            head_ = head_->next;
            delete removed;
        }
        tail_ = nullptr;
        size_ = 0;
    }

    std::size_t size() const { return size_; }
    bool empty() const { return head_ == nullptr; }

    void print() const {
        std::cout << "[";
        bool first = true;
        for (ConstIterator it = begin(); it != end(); ++it) {
            if (!first) std::cout << ", ";
            std::cout << *it;
            first = false;
        }
        std::cout << "]\n";
    }

    void printReverse() const {
        std::cout << "[";
        bool first = true;
        Node* current = tail_;
        while (current != nullptr) {
            if (!first) std::cout << ", ";
            std::cout << current->data;
            first = false;
            current = current->prev;
        }
        std::cout << "]\n";
    }

    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }
    ConstIterator begin() const { return ConstIterator(head_); }
    ConstIterator end() const { return ConstIterator(nullptr); }

    void swap(DoublyLinkedList& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

    bool operator==(const DoublyLinkedList& other) const {
        if (size_ != other.size_) return false;
        ConstIterator left = begin();
        ConstIterator right = other.begin();
        while (left != end()) {
            if (*left != *right) return false;
            ++left;
            ++right;
        }
        return true;
    }

    bool operator!=(const DoublyLinkedList& other) const { return !(*this == other); }
};

#endif
