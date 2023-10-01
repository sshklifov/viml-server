#pragma once

#include <Range.hpp>
#include <Vector.hpp>

struct Locator {
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

    Range resolve(int begin) const {
        return resolve(begin, begin + 1);
    }

    Range resolve(int begin, int end) const {
        assert(!fragments.empty());
        assert(begin < end);
        Range res;
        int strBegin = 0;
        for (int i = 0; i < fragments.count(); ++i) {
            int strEnd = strBegin + fragments[i].fragLen;
            if (begin >= strBegin && begin < strEnd) {
                res.start.line = fragments[i].line;
                res.start.character = fragments[i].col + (begin - strBegin);
            }
            if (end > strBegin && end <= strEnd) {
                res.end.line = fragments[i].line;
                res.end.character = fragments[i].col + (end - strBegin);
                return res;
            }
            strBegin = strEnd;
        }
        if (res.start.line == MAXLNUM) {
            res.start.line = fragments.last().line;
            res.start.character = fragments.last().col + fragments.last().fragLen - 1;
        }
        res.end.line = fragments.last().line;
        res.end.character = fragments.last().col + fragments.last().fragLen;
        return res;
    }

    Range resolve() const {
        assert(!fragments.empty());
        Range res;
        res.start.line = fragments[0].line;
        res.start.character = fragments[0].col;
        res.end.line = fragments.last().line;
        res.end.character = fragments.last().col + fragments.last().fragLen - 1;
        return res;
    }

private:
    Vector<Fragment> fragments;
};
