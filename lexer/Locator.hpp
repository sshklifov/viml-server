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

    Range resolve(int first, int last) const {
        assert(first <= last);
        Range res;
        int strBegin = 0;
        for (int i = 0; i < fragments.count(); ++i) {
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
        Position bad(MAXLNUM, MAXCOL);
        return Range(bad);
    }

    Range resolve() const {
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
