#pragma once

#include <cstring>
#include <cassert>

struct StringView {
    StringView() : begin(nullptr), end(nullptr) {}
    explicit StringView(const char* s) : begin(s), end(s + strlen(begin)) {}
    StringView(const char* begin, const char* end) : begin(begin), end(end) {}
    StringView(const char* s, int n) : begin(s), end(s + n) {}

    // Access

    bool empty() const {
        return begin >= end;
    }

    int length() const {
        return end - begin;
    }

    char operator[](int idx) const {
        return begin[idx];
    }

    char left() const { return *begin; }
    char right() const { return *(end - 1); }

    bool beginsWith(char c) const {
        return !empty() && left() == c;
    }

    bool endsWith(char c) const {
        return !empty() && right() == c;
    }

    bool beginsWith(const char* prefix) const {
        const char* it = begin;
        while (it < end && *prefix) {
            if (*prefix != *it) {
                return false;
            }
            ++it;
            ++prefix;
        }
        return *prefix == '\0';
    }

    // Iterators

    const char* find(char c) const {
        return find(begin, c);
    }

    const char* find(const char* it, char c) const {
        assert(it >= begin && it <= end);
        while (it < end) {
            if (*it == c) {
                return it;
            }
            ++it;
        }
        return end;
    }

    const char* find(char c1, char c2) const {
        const char* it = begin;
        while (it < end) {
            if (*it == c1 || *it == c2) {
                return it;
            }
            ++it;
        }
        return end;
    }

    // Generators

    StringView trunc(int newSize) const {
        assert(newSize >= 0);
        if (newSize < length()) {
            return StringView(begin, begin + newSize);
        } else {
            return (*this);
        }
    }

    StringView popLeft(int count = 1) const {
        return StringView(begin + count, end);
    }

    StringView popRight(int count = 1) const {
        return StringView(begin, end - count);
    }

    StringView trimLineFeed() const {
        StringView res(*this);
        if (res.endsWith('\n')) {
            res = popRight();
            if (res.endsWith('\r')) {
                res = popRight();
            }
        }
        return res;
    }

    StringView trimLeftSpace() const {
        const char* it = begin;
        while (it < end) {
            if (*it != ' ' && *it != '\t') {
                break;
            }
            ++it;
        }
        return StringView(it, end);
    }

    StringView trimRightSpace() const {
        const char* it = end - 1;
        while (it >= begin) {
            if (*it != ' ' && *it != '\t') {
                break;
            }
            ++it;
        }
        return StringView(begin, it + 1);
    }

    StringView trimSpace() {
        return trimLeftSpace().trimRightSpace();
    }

    StringView popLineFeed() {
        if (!empty() && right() == '\n') {
            return StringView(begin, end - 1);
        } else {
            return *this;
        }
    }

    // Comparators

    int cmp(StringView other) const {
        StringView copy(*this);
        while (!copy.empty() && !other.empty()) {
            int c = *copy.begin - *other.begin;
            if (c != 0) {
                return c;
            }
            ++copy.begin;
            ++other.begin;
        }
        return copy.length() - other.length();
    }

    bool operator<(const StringView& other) const {
        return cmp(other) < 0;
    }
    bool operator>(const StringView& other) const {
        return cmp(other) > 0;
    }
    bool operator<=(const StringView& other) const {
        return cmp(other) <= 0;
    }
    bool operator>=(const StringView& other) const {
        return cmp(other) >= 0;
    }
    bool operator==(const StringView& other) const {
        return cmp(other) == 0;
    }
    bool operator!=(const StringView& other) const {
        return cmp(other) != 0;
    }

    const char* begin;
    const char* end;
};
