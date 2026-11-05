#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include <cstddef>
#include <stdexcept>

template <typename T>
class Vector {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void grow() {
        std::size_t newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
        T* newData = new T[newCapacity];
        for (std::size_t i = 0; i < size_; ++i) newData[i] = data_[i];
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

public:
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit Vector(std::size_t count)
        : data_(count == 0 ? nullptr : new T[count]),
          size_(count), capacity_(count) {}

    ~Vector() { delete[] data_; }

    void push_back(const T& value) {
        if (size_ == capacity_) grow();
        data_[size_++] = value;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("Vector is empty");
        --size_;
    }

    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }

    void clear() { size_ = 0; }

    T& operator[](std::size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }
};

#endif
