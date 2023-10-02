#pragma once

#include <utility>
#include <cstdlib>
#include <cassert>

template <typename T>
struct Vector {
    Vector() : arr(nullptr), len(0), allocLen(0) {}

    Vector(const Vector& rhs) : Vector() {
        copy(rhs);
    }

    Vector(Vector&& rhs) : arr(rhs.arr), len(rhs.len), allocLen(rhs.allocLen) {
        rhs.arr = nullptr;
        rhs.len = 0;
        rhs.allocLen = 0;
    }

    ~Vector() {
        clear();
        if (arr) {
            free(arr);
        }
    }

    Vector& operator=(const Vector& rhs) {
        copy(rhs);
        return *this;
    }

    template <typename... Types>
    void emplace(Types&&... args) {
        allocAtLeast(len + 1);
        new(arr + len) T(std::forward<Types>(args)...);
        ++len;
    }

    void clear() {
        if (!std::is_trivially_destructible<T>::value) {
            for (int i = 0; i < len; ++i) {
                arr[i].~T();
            }
        }
        len = 0;
    }

    void copy(const Vector& rhs) {
        clear();
        allocAtLeast(rhs.len);
        for (int i = 0; i < rhs.len; ++i) {
            new(arr + i) T(rhs.arr[i]);
        }
        len = rhs.len;
    }

    bool empty() const { return len == 0; }

    void remove(int pos) {
        assert(pos < len);
        if (!std::is_trivially_destructible<T>::value) {
            arr[pos].~T();
        }
        if (pos != len - 1) {
            memcpy(arr + pos, arr + len - 1, sizeof(T));
        }
        --len;
    }

    void removeLast() { remove(len - 1); }

    int count() const { return len; }

    T& operator[](int i) { return arr[i]; }
    const T& operator[](int i) const { return arr[i]; }

    T& first() { return arr[0]; }
    const T& first() const { return arr[0]; }
    T& last() { return arr[len - 1]; }
    const T& last() const { return arr[len - 1]; }

    T* begin() { return arr; }
    T* end() { return arr + len; }

    const T* begin() const { return arr; }
    const T* end() const { return arr + len; }

    void resize(int n) {
        clear();
        allocAtLeast(n);
        if (!std::is_trivially_constructible<T>::value) {
            for (int i = 0; i < n; ++i) {
                new(arr + i) T();
            }
        }
        len = n;
    }

    void allocAtLeast(int n) {
        if (n < allocLen) {
            return;
        }
        int doubleAllocLen = allocLen * 2;
        allocLen = (doubleAllocLen > n) ? doubleAllocLen : n;
        arr = (T*)realloc(arr, sizeof(T) * allocLen);
    }

private:
    T* arr;
    int len;
    int allocLen;
};
