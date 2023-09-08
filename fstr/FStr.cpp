#include "FStr.hpp"

#include <cstring>

FStr::FStr() : s(nullptr), len(0), allocLen(0) {}

FStr::FStr(FStr&& rhs) :
    s(rhs.s), len(rhs.len), allocLen(rhs.allocLen) {
    rhs.s = nullptr;
    rhs.len = 0;
    rhs.allocLen = 0;
}

FStr& FStr::operator=(FStr&& rhs) {
    delete[] s;
    s = rhs.s;
    rhs.s = nullptr;

    len = rhs.len;
    allocLen = rhs.allocLen;

    return *this;
}

void FStr::append(const char* s) {
    int reqLen = len + strlen(s);
    allocAtLeast(reqLen);
    appendNoCheck(s);
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
