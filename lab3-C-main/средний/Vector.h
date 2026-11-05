#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <utility>

// Небольшая учебная реализация динамического массива.
template <typename T>
class Vector {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    static void copyElements(T* destination, const T* source, std::size_t count) {
        for (std::size_t i = 0; i < count; ++i) {
            destination[i] = source[i];
        }
    }

public:
    using iterator = T*;
    using const_iterator = const T*;

    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit Vector(std::size_t count)
        : data_(count == 0 ? nullptr : new T[count]),
          size_(count), capacity_(count) {}

    Vector(std::size_t count, const T& value)
        : data_(count == 0 ? nullptr : new T[count]),
          size_(count), capacity_(count) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }

    // Глубокое копирование.
    Vector(const Vector& other)
        : data_(other.capacity_ == 0 ? nullptr : new T[other.capacity_]),
          size_(other.size_), capacity_(other.capacity_) {
        copyElements(data_, other.data_, size_);
    }

    // Перемещение забирает уже выделенную память.
    Vector(Vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~Vector() {
        delete[] data_;
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) { // защита от самоприсваивания
            Vector copy(other);
            swap(copy);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
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

    T& operator[](std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    void reserve(std::size_t newCapacity) {
        if (newCapacity <= capacity_) {
            return;
        }

        T* newData = new T[newCapacity];
        copyElements(newData, data_, size_);
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void pop_back() {
        if (empty()) {
            throw std::underflow_error("Vector is empty");
        }
        --size_;
    }

    void resize(std::size_t newSize, const T& value = T()) {
        if (newSize > capacity_) {
            reserve(newSize);
        }
        for (std::size_t i = size_; i < newSize; ++i) {
            data_[i] = value;
        }
        size_ = newSize;
    }

    void clear() { size_ = 0; }

    void reverse() {
        for (std::size_t left = 0, right = size_ == 0 ? 0 : size_ - 1;
             left < right; ++left, --right) {
            std::swap(data_[left], data_[right]);
        }
    }

    void swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    iterator begin() { return data_; }
    iterator end() { return data_ == nullptr ? nullptr : data_ + size_; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ == nullptr ? nullptr : data_ + size_; }
    const_iterator cbegin() const { return data_; }
    const_iterator cend() const { return data_ == nullptr ? nullptr : data_ + size_; }
};

#endif
