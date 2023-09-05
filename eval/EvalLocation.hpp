#pragma once

#include <ostream>
#include <cassert>

struct EvalLocation {
    EvalLocation() = default;
    EvalLocation(int begin, int end) : begin(begin), end(end) {}

    int begin;
    int end;
};

inline std::ostream& operator<<(std::ostream& o, const EvalLocation& l) {
    assert(false);
    return o;
}
