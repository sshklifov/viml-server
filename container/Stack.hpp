#pragma once

#include <Vector.hpp>

template <typename T>
struct Stack {
    T& top() { return s.last(); }
    const T& top() const { return s.last(); }

    void pop() { s.removeLast(); }

    template <typename... Types>
    void emplace(Types&&... args) {
        s.emplace(std::forward<Types>(args)...);
    }

private:
    Vector<T> s;
};
