#pragma once

#include <cstring>

struct StringView {
    StringView() : begin(nullptr), end(nullptr) {}
    explicit StringView(const char* s) : begin(s), end(s + strlen(begin)) {}
    StringView(const char* begin, const char* end) : begin(begin), end(end) {}
    StringView(const char* s, int n) : begin(s), end(s + n) {}

    bool empty() const {
        return begin >= end;
    }

    int length() const {
        return end - begin;
    }

    bool truncate(int newSize) {
        if (newSize >= 0 && newSize < length()) {
            end = begin + newSize;
            return true;
        }
        return false;
    }

    char operator[](int idx) {
        return begin[idx];
    }

    StringView substr(int substrBegin, int substrEnd) const {
        if (substrBegin >= substrEnd) {
            return StringView();
        }
        StringView res(begin + substrBegin, begin + substrEnd);
        if (res.begin > end || res.end < begin) {
            return StringView();
        }
        return res;
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

    int cmpn(StringView other, int n) const {
        StringView copy(*this);
        copy.truncate(n);
        other.truncate(n);
        return copy.cmp(other);
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
