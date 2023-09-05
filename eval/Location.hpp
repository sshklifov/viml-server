#pragma once

#include <ostream>

struct Location {
    Location() = default;
    Location(int begin, int end) : begin(begin), end(end) {}

    int begin;
    int end;
};

inline std::ostream& operator<<(std::ostream& o, const Location& l) {
    return o; // TODO
}
