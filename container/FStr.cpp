#include "FStr.hpp"
#include "BitTwiddle.hpp"

#include <cstring>

FStr::FStr() : s(nullptr), len(0), allocLen(0) {}

FStr::FStr(const char* s) : FStr() {
    if (*s) {
        append(s);
    }
}

FStr::FStr(const char* s, int n) : FStr() {
    if (*s && n) {
        append(s, n);
    }
}

FStr::FStr(const char* begin, const char* end) : FStr(begin, end - begin) {}

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

void FStr::append(void* ptr) {
    allocAtLeast(len + 18);
    uint64_t addr = (uint64_t)ptr;
    s[len++] = '0';
    s[len++] = 'x';

    if (addr == 0) {
        s[len++] = '0';
    } else {
        unsigned shiftBase = (logOfTwo(addr) >> 2) << 2;
        while (shiftBase) {
            unsigned dig = addr >> shiftBase;
            if (dig <= 9) {
                s[len++] = '0' + dig;
            } else {
                s[len++] = 'a' + (dig - 10);
            }
            addr -= (static_cast<uint64_t>(dig) << shiftBase);
            shiftBase -= 4;
        }
    }
    s[len] = '\0';
}

void FStr::append(const char* other) {
    if (other) {
        append(other, strlen(other));
    }
}

void FStr::append(const char* other, int n) {
    allocAtLeast(len + n);
    strncpy(s + len, other, n);
    len += n;
    s[len] = '\0';
}

void FStr::append(int d) {
    allocAtLeast(len + 11);

    if (d == 0) {
        s[len] = '0';
    } else {
        if (d < 0) {
            s[len++] = '-';
            d = -d;
        }
        int divBase = prevPowerOfTen(d);
        while (d > 0) {
            int dig = d / divBase;
            s[len++] = '0' + dig;
            d -= divBase * dig;
            divBase /= 10;
        }
    }

    s[len] = '\0';
}

void FStr::append(unsigned u) {
    allocAtLeast(len + 10);
    if (u == 0) {
        s[len++] = '0';
    } else {
        int divBase = prevPowerOfTen(u);
        while (u > 0) {
            unsigned dig = u / divBase;
            s[len++] = '0' + dig;
            u -= divBase * dig;
            divBase /= 10;
        }
    }

    s[len] = '\0';
}

void FStr::append(const FStr& other) {
    append(other.s, other.len);
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

bool FStr::operator==(const char* other) const {
    return strcmp(s, other) == 0;
}

bool FStr::operator!=(const char* other) const {
    return strcmp(s, other) != 0;
}

bool FStr::operator==(const FStr& other) const {
    return *this == other.s;
}

bool FStr::operator!=(const FStr& other) const {
    return *this != other.s;
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

void FStr::clear() {
    len = 0;
}

const char* FStr::str() const { return s; }

int FStr::length() const {
    return len;
}

bool FStr::empty() const {
    return len == 0;
}

void FStr::allocAtLeast(int n) {
    n += 1; // For terminating NULL
    if (n < allocLen) {
        return;
    }
    int doubleAllocLen = allocLen * 2;
    allocLen = (doubleAllocLen > n) ? doubleAllocLen : n;

    char* old = s;
    s = new char[n];
    strncpy(s, old, len);
    s[len] = '\0';
    delete[] old;
}
