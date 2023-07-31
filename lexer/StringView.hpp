#pragma once

#include <cstring>

struct StringView {
    StringView() : begin(nullptr), end(nullptr) {}
    explicit StringView(const char* s) : begin(s), end(s + strlen(begin)) {}
    StringView(const char* begin, const char* end) : begin(begin), end(end) {}
    StringView(const char* s, int n) : begin(s), end(s + n) {}

    bool mergeAdjacent(const StringView& adj) {
        if (end == adj.begin) {
            end = adj.end;
            return true;
        }
        if (begin == adj.end) {
            begin = adj.begin;
            return true;
        }
        return false;
    }

    bool empty() const {
        return begin == end;
    }

    int length() const {
        return end - begin;
    }

    bool shrink(int newSize) {
        if (newSize < length()) {
            end = begin + newSize;
            return true;
        }
        return false;
    }

    StringView substring(int n) const {
        if (n > length()) {
            return StringView();
        } else {
            return StringView(begin, begin + n);
        }
    }

    bool split(int n, StringView& lhs, StringView& rhs) const {
        if (n > length()) {
            return false;
        }
        lhs = StringView(begin, begin + n);
        rhs = StringView(begin + n, end);
        return true;
    }

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

    char front() const { return *begin; }
    char back() const { return *(end - 1); }

    const char* begin;
    const char* end;
};
