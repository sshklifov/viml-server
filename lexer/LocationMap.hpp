#pragma once

#include <vector>

struct LocationMap {
    friend struct Continuation;

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

    void addEntry(int line, int col, int entryLen) {
        locations.push_back(Location(line, col, entryLen));
    }

    void clearEntries() {
        locations.clear();
    }

    bool resolve(const Key& key, int strOffset, int& line, int& col) const {
        int strBegin = 0;
        for (int i = key.entryBegin; i < key.entryEnd; ++i) {
            int strEnd = strBegin + locations[i].entryLen;
            if (strOffset >= strBegin && strOffset < strEnd) {
                line = locations[i].line;
                col = locations[i].col + (strOffset - strBegin);
                return true;
            }
            strBegin = strEnd;
        }
        return false;
    }

private:
    std::vector<Location> locations;
};
