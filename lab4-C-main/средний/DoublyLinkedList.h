#ifndef MEDIUM_DOUBLY_LIST_H
#define MEDIUM_DOUBLY_LIST_H

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        Node* prev;
        Node* next;
        explicit Node(const T& v) : value(v), prev(nullptr), next(nullptr) {}
    };
    Node* head_;
    Node* tail_;
    std::size_t size_;

public:
    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    ~DoublyLinkedList() { clear(); }

    void push_back(const T& value) {
        Node* node = new Node(value);
        node->prev = tail_;
        if (tail_ != nullptr) tail_->next = node;
        else head_ = node;
        tail_ = node;
        ++size_;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("List is empty");
        Node* removed = tail_;
        tail_ = tail_->prev;
        if (tail_ != nullptr) tail_->next = nullptr;
        else head_ = nullptr;
        delete removed;
        --size_;
    }

    // Вариант 2 среднего уровня: слияние отсортированных списков.
    void merge(DoublyLinkedList& other) {
        DoublyLinkedList result;
        Node* left = head_;
        Node* right = other.head_;
        while (left != nullptr && right != nullptr) {
            if (left->value <= right->value) {
                result.push_back(left->value);
                left = left->next;
            } else {
                result.push_back(right->value);
                right = right->next;
            }
        }
        while (left != nullptr) { result.push_back(left->value); left = left->next; }
        while (right != nullptr) { result.push_back(right->value); right = right->next; }
        clear();
        swap(result);
        other.clear();
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

    void printReverse() const {
        std::cout << "[";
        Node* current = tail_;
        while (current != nullptr) {
            std::cout << current->value;
            if (current->prev != nullptr) std::cout << ", ";
            current = current->prev;
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

    void swap(DoublyLinkedList& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }
};

#endif
