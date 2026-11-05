#ifndef BASIC_SINGLY_LIST_H
#define BASIC_SINGLY_LIST_H

#include <cstddef>
#include <iostream>
#include <stdexcept>

template <typename T>
class SinglyLinkedList {
private:
    struct Node { T value; Node* next; explicit Node(const T& v) : value(v), next(nullptr) {} };
    Node* head_;
    Node* tail_;
    std::size_t size_;

public:
    SinglyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    ~SinglyLinkedList() { clear(); }

    void push_back(const T& value) {
        Node* node = new Node(value);
        if (tail_ == nullptr) head_ = tail_ = node;
        else { tail_->next = node; tail_ = node; }
        ++size_;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("List is empty");
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

    void print() const {
        std::cout << "[";
        Node* current = head_;
        while (current != nullptr) {
            std::cout << current->value;
            if (current->next != nullptr) std::cout << ", ";
            current = current->next;
        }
        std::cout << "]\n";
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void clear() noexcept {
        while (head_ != nullptr) {
            Node* removed = head_;
            head_ = head_->next;
            delete removed;
        }
        tail_ = nullptr;
        size_ = 0;
    }
};

#endif
