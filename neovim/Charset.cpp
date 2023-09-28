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

const char* skipwhite_len(const char *p, size_t len) {
    while (len > 0 && ascii_iswhite(*p)) {
        p++;
        len--;
    }
    return p;
}

const char* skipwhite(const char* p) {
    while (ascii_iswhite(*p)) {
        p++;
    }
    return p;
}

const char *skipdigits(const char *q) {
    const char *p = q;
    while (ascii_isdigit(*p)) {
        // skip to next non-digit
        p++;
    }
    return p;
}

const char *skiptowhite(const char *p) {
    while (*p != ' ' && *p != '\t' && *p != NUL) {
        p++;
    }
    return p;
}
