#include "FStr.hpp"

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
    appendNoCheck(c);
}

void FStr::append(const char* s) {
    int reqLen = len + strlen(s);
    allocAtLeast(reqLen);
    appendNoCheck(s);
}

void FStr::append(const FStr& other) {
    int reqLen = len + other.length();
    allocAtLeast(reqLen);
    appendNoCheck(other.str());
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

const char* FStr::str() const { return s; }

int FStr::length() const {
    return len;
}

void FStr::allocAtLeast(int n) {
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
