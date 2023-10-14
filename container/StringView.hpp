#pragma once


#include <cstring>
#include <cassert>

struct StringView {
    StringView() : begin(nullptr), end(nullptr) {}
    explicit StringView(const char* s) : begin(s), end(s + strlen(s)) {}

    StringView(const char* begin, const char* end) : begin(begin), end(end) {}
    StringView(const char* s, int n) : begin(s), end(s + n) {}

    bool empty() const { return begin == end; }
    int length() const { return end - begin; }

    char operator[](int idx) const { return begin[idx]; }

    int cmp(StringView rhs) const {
        StringView lhs(*this);
        while (!lhs.empty() && !rhs.empty()) {
            int c = *lhs.begin - *rhs.begin;
            if (c != 0) {
                return c;
            }
            ++lhs.begin;
            ++rhs.begin;
        }
        return lhs.length() - rhs.length();
    }

    bool operator<(const StringView& other) const  { return cmp(other) <  0; }
    bool operator>(const StringView& other) const  { return cmp(other) >  0; }
    bool operator<=(const StringView& other) const { return cmp(other) <= 0; }
    bool operator>=(const StringView& other) const { return cmp(other) >= 0; }
    bool operator==(const StringView& other) const { return cmp(other) == 0; }
    bool operator!=(const StringView& other) const { return cmp(other) != 0; }

    const char* begin;
    const char* end;
};
