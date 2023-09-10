#include "StringView.hpp"

StringView::StringView() : begin(nullptr), end(nullptr) {}

StringView::StringView(const char* s) : begin(s), end(s + strlen(begin)) {}

StringView::StringView(const char* begin, const char* end) : begin(begin), end(end) {}

StringView::StringView(const char* s, int n) : begin(s), end(s + n) {}

bool StringView::empty() const {
    return begin >= end;
}

int StringView::length() const {
    return end - begin;
}

char StringView::operator[](int idx) const {
    return begin[idx];
}

char StringView::left() const {
    return *begin;
}

char StringView::right() const {
    return *(end - 1);
}

bool StringView::beginsWith(char c) const {
    return !empty() && left() == c;
}

bool StringView::endsWith(char c) const {
    return !empty() && right() == c;
}

bool StringView::beginsWith(const char* prefix) const {
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

const char* StringView::find(char c) const {
    return find(begin, c);
}

const char* StringView::find(const char* it, char c) const {
    assert(it >= begin && it <= end);
    while (it < end) {
        if (*it == c) {
            return it;
        }
        ++it;
    }
    return end;
}

const char* StringView::find(char c1, char c2) const {
    const char* it = begin;
    while (it < end) {
        if (*it == c1 || *it == c2) {
            return it;
        }
        ++it;
    }
    return end;
}

StringView StringView::trunc(int newSize) const {
    assert(newSize >= 0);
    if (newSize < length()) {
        return StringView(begin, begin + newSize);
    } else {
        return (*this);
    }
}

StringView StringView::popLeft(int count) const {
    return StringView(begin + count, end);
}

StringView StringView::popRight(int count) const {
    return StringView(begin, end - count);
}

StringView StringView::trimLineFeed() const {
    StringView res(*this);
    if (res.endsWith('\n')) {
        res = popRight();
        if (res.endsWith('\r')) {
            res = popRight();
        }
    }
    return res;
}

StringView StringView::trimLeftSpace() const {
    const char* it = begin;
    while (it < end) {
        if (*it != ' ' && *it != '\t') {
            break;
        }
        ++it;
    }
    return StringView(it, end);
}

StringView StringView::trimRightSpace() const {
    const char* it = end - 1;
    while (it >= begin) {
        if (*it != ' ' && *it != '\t') {
            break;
        }
        ++it;
    }
    return StringView(begin, it + 1);
}

StringView StringView::trimSpace() const {
    return trimLeftSpace().trimRightSpace();
}

StringView StringView::popLineFeed() const {
    if (!empty() && right() == '\n') {
        return StringView(begin, end - 1);
    } else {
        return *this;
    }
}

int StringView::cmp(StringView other) const {
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

bool StringView::operator<(const StringView& other) const {
    return cmp(other) < 0;
}

bool StringView::operator>(const StringView& other) const {
    return cmp(other) > 0;
}

bool StringView::operator<=(const StringView& other) const {
    return cmp(other) <= 0;
}

bool StringView::operator>=(const StringView& other) const {
    return cmp(other) >= 0;
}

bool StringView::operator==(const StringView& other) const {
    return cmp(other) == 0;
}

bool StringView::operator!=(const StringView& other) const {
    return cmp(other) != 0;
}
