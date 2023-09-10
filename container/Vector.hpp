#pragma once

#include <utility>
#include <cstdlib>
#include <cassert>

template <typename T>
struct Vector {
    Vector() : arr(nullptr), len(0), allocLen(0) {}

    Vector(const Vector& rhs) : Vector() {
        allocAtLeast(rhs.len);
        for (int i = 0; i < rhs.len; ++i) {
            new(arr + i) T(rhs.arr[i]);
        }
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

    template <typename... Types>
    void emplace(Types&&... args) {
        allocAtLeast(len + 1);
        new(arr + len) T(std::forward<Types>(args)...);
        ++len;
    }

    void clear() {
        for (int i = 0; i < len; ++i) {
            arr[i].~T();
        }
        len = 0;
    }

    void remove(int pos) {
        assert(pos < len);
        arr[pos].~T();
        if (pos != len - 1) {
            memcpy(arr + pos, arr + len - 1, sizeof(T));
        }
        --len;
    }

    int count() const { return len; }

    T& operator[](int i) { return arr[i]; }
    const T& operator[](int i) const { return arr[i]; }

private:
    void allocAtLeast(int n) {
        if (n < allocLen) {
            return;
        }
        int doubleAllocLen = allocLen * 2;
        allocLen = (doubleAllocLen > n) ? doubleAllocLen : n;
        arr = (T*)realloc(arr, sizeof(T) * allocLen);
    }

    T* arr;
    int len;
    int allocLen;
};
