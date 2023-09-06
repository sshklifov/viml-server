#pragma once

#include <vector>

struct LocationMap {
    struct Key {
        Key() = default;
        Key(int begin, int end) : entryBegin(begin), entryEnd(end) {}

        int entryBegin;
        int entryEnd;
    };

    struct Location {
        Location() = default;
        Location(int line, int col, int entryLen) : line(line), col(col), entryLen(entryLen) {}

        int line;
        int col;
        int entryLen;
    };

    bool resolve(const Key& key, int strOffset, int& line, int& col) const {
        int strBegin = 0;
        for (int i = key.entryBegin; i < key.entryEnd; ++i) {
            int strEnd = strBegin + locations[i].entryLen;
            if (strOffset >= strBegin && strOffset < strEnd) {
                line = locations[i].line;
                col = locations[i].col + (strOffset - strBegin);
                // Convert to 1-based indices
                ++line;
                ++col;
                return true;
            }
            strBegin = strEnd;
        }
        return false;
    }

    std::vector<Location> locations;
};
