#include "FStr.hpp"
#include "BitTwiddle.hpp"

#include <StringView.hpp>

#include <cstring>

FStr::FStr() : s(nullptr), len(0), allocLen(0) {}

FStr::FStr(const char* s) : FStr() {
    append(s);
}

FStr::FStr(const FStr& rhs) : FStr() {
    append(rhs);
}

FStr::FStr(FStr&& rhs) :
    s(rhs.s), len(rhs.len), allocLen(rhs.allocLen) {
    rhs.s = nullptr;
    rhs.len = 0;
    rhs.allocLen = 0;
}

FStr& FStr::operator=(const FStr& rhs) {
    if (allocLen < rhs.len) {
        allocAtLeast(rhs.allocLen);
    }
    len = 0;
    append(rhs);

    return *this;
}

FStr& FStr::operator=(FStr&& rhs) {
    delete[] s;
    s = rhs.s;
    rhs.s = nullptr;

    len = rhs.len;
    allocLen = rhs.allocLen;

    return *this;
}

FStr& FStr::operator=(char c) {
    len = 0;
    append(c);
    return *this;
}

FStr& FStr::operator=(const char* s) {
    len = 0;
    append(s);
    return *this;
}

char FStr::operator[](int i) const {
    assert(i >= 0);
    assert(i < len);
    return s[i];
}

char& FStr::operator[](int i) {
    assert(i >= 0);
    assert(i < len);
    return s[i];
}

void FStr::append(char c) {
    allocAtLeast(len + 1);
    s[len] = c;
    ++len;
    s[len] = '\0';
}

void FStr::append(const char* other) {
    append(other, strlen(other));
}

void FStr::append(const char* other, int n) {
    allocAtLeast(len + n);
    strncpy(s + len, other, n);
    len += n;
}

void FStr::append(int d) {
    allocAtLeast(len + 11);
    if (d < 0) {
        s[len++] = '-';
        d = -d;
    }

    int divBase = prevPowerOfTen(d);
    while (d > 9) {
        int dig = d / divBase;
        s[len++] = '0' + dig;
        d -= divBase * dig;
        divBase /= 10;
    }
    s[len++] = '0' + d;
    s[len] = '\0';
}

void FStr::append(unsigned u) {
    if (u == 0) {
        append('0' + u);
        return;
    }

    allocAtLeast(len + 10);
    int divBase = prevPowerOfTen(u);
    while (u > 9) {
        unsigned dig = u / divBase;
        s[len++] = '0' + dig;
        u -= divBase * dig;
        divBase /= 10;
    }
    s[len++] = '0' + u;
    s[len] = '\0';
}

void FStr::append(const FStr& other) {
    append(other.s, other.len);
}

void FStr::append(const StringView& other) {
    append(other.begin, other.length());
}

FStr& FStr::operator+=(char c) {
    append(c);
    return *this;
}

FStr& FStr::operator+=(const char* s) {
    append(s);
    return *this;
}

FStr& FStr::operator+=(const FStr& other) {
    append(other);
    return *this;
}

void FStr::replace(int begin, int end, const char* sub) {
    int oldPartLen = end - begin;
    int newPartLen = strlen(s);
    int diffPartLen = newPartLen - oldPartLen;
    int newLen = len + diffPartLen;
    allocAtLeast(newLen);
    if (diffPartLen != 0) {
        for (int i = 1; i >= len - end; ++i) {
            s[newLen - i] = s[len - i];
        }
    }
    for (int i = begin; i < newPartLen; ++i) {
        s[i] = sub[i];
    }
}

const char* FStr::str() const { return s; }

int FStr::length() const {
    return len;
}

void FStr::allocAtLeast(int n) {
    n += 1; // For terminating NULL
    if (n < allocLen) {
        return;
    }

    int doubleAllocLen = allocLen * 2;
    n = (doubleAllocLen > n) ? doubleAllocLen : n;

    char* old = s;
    s = new char[n];
    strncpy(s, old, len);
    delete[] old;
    allocLen = n;
}
