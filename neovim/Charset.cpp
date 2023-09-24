#include "Charset.hpp"
#include "Ascii.hpp"

#include <cctype>
#include <cstring>
#include <cassert>

bool vim_isIDc(int c) {
    return isalnum(c) || c == '_';
}

bool vim_iswordc(const int c) {
    return vim_isIDc(c);
}

const char *skipwhite_len(const char *p, std::size_t len) {
    for (; len > 0 && ascii_iswhite(*p); len--) {
        p++;
    }
    return p;
}

const char *skipwhite(const char *const p) {
    return skipwhite_len(p, strlen(p));
}

const char *skipdigits(const char *q) {
    const char *p = q;
    while (ascii_isdigit(*p)) {
        // skip to next non-digit
        p++;
    }
    return p;
}
