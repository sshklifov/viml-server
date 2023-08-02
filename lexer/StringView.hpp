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

    char operator[](int idx) {
        return begin[idx];
    }

    char front() const { return *begin; }
    char back() const { return *(end - 1); }

    // Mutators

    StringView truncated(int newSize) const {
        assert(newSize >= 0);
        if (newSize < length()) {
            return StringView(begin, begin + newSize);
        } else {
            return (*this);
        }
    }

    StringView substr(int beginOffset, int endOffset) const {
        assert(beginOffset >= 0 && endOffset >= 0);
        assert(beginOffset <= endOffset);
        assert(begin + beginOffset < end);
        assert(begin + endOffset <= end);
        return StringView(begin + beginOffset, begin + endOffset);
    }

    StringView trim(const char* all) {
        const char* newBegin = begin;
        while (newBegin < end) {
            int found = false;
            for (int i = 0; all[i]; ++i) {
                if (*newBegin == all[i] ) {
                    found = true;
                    break;
                }
            }
            if (found) {
                ++newBegin;
            } else {
                break;
            }
        }
        return StringView(newBegin, end);
    }

    StringView trimUntil(const char* any) {
        const char* newBegin = begin;
        while (newBegin < end) {
            int found = false;
            for (int i = 0; any[i]; ++i) {
                if (*newBegin == any[i] ) {
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            } else {
                ++newBegin;
            }
        }
        return StringView(newBegin, end);
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
