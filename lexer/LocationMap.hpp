#pragma once

#include <Range.hpp>
#include <Vector.hpp>

struct LocationMap {
    friend struct Continuation;

    struct Key {
        Key() = default;
        Key(int begin, int end) : fragBegin(begin), fragEnd(end) {}

        int fragBegin;
        int fragEnd;
    };

    struct Fragment {
        Fragment() = default;
        Fragment(int line, int col, int fragLen) : line(line), col(col), fragLen(fragLen) {}

        int line;
        int col;
        int fragLen;
    };

    void addFragment(int line, int col, int entryLen) {
        fragments.emplace(line, col, entryLen);
    }

    void clearFlagments() {
        fragments.clear();
    }

    Range resolve(const Key& key, int first, int last) const {
        Range res;
        int strBegin = 0;
        for (int i = key.fragBegin; i < key.fragEnd; ++i) {
            int strEnd = strBegin + fragments[i].fragLen;
            if (first >= strBegin && first < strEnd) {
                res.start.line = fragments[i].line;
                res.start.character = fragments[i].col + (first - strBegin);
            }
            if (last >= strBegin && last < strEnd) {
                res.end.line = fragments[i].line;
                res.end.character = fragments[i].col + (last - strBegin);
                return res;
            }
            strBegin = strEnd;
        }
        assert(false);
        return res;
    }

    Range resolve(const Key& key) const {
        Range res;
        res.start.line = fragments[key.fragBegin].line;
        res.start.character = fragments[key.fragBegin].col;
        res.end.line = fragments[key.fragEnd].line;
        res.end.character = fragments[key.fragEnd].col;
        return res;
    }

private:
    Vector<Fragment> fragments;
};
