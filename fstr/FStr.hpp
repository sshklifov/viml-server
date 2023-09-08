#pragma once

#include "FStrUtil.hpp"

template <typename... Ints>
int sum() {
    return 0;
}

template <typename... Ints>
int sum(int first, Ints... rest) {
    return first + sum(rest...);
}

struct FStr {
    FStr();
    FStr(const char* s);
    FStr(const FStr& rhs);
    FStr(FStr&& rhs);

    FStr& operator=(const FStr& rhs);
    FStr& operator=(FStr&& rhs);

    char operator[](int i) const;
    char& operator[](int i);

    void append(char c);
    void append(const char* s);
    void append(const FStr& other);

    FStr& operator=(char c);
    FStr& operator=(const char* s);

    FStr& operator+=(char c);
    FStr& operator+=(const char* s);
    FStr& operator+=(const FStr& other);

    template <typename... Types>
    void appendf(const char* fmt, const Types&... args) {
        int fmtLen = charsNeeded(fmt, args...);
        int totalLen = len + fmtLen;
        allocAtLeast(totalLen + 1); //< For terminating null
        fNoCheck(fmt, args...);
    }

    const char* str() const;

    int length() const;

private:
    void allocAtLeast(int n);

    template <typename T, typename... Types>
    void fNoCheck(const char* fmt, const T& head, const Types&... tail) {
        if (fmt[0] == '\0') {
            assert(false && "Too many arguments");
            s[len] = '\0';
            return;
        }
        if (fmt[0] == '{' && fmt[1] == '}') {
            appendNoCheck(head);
            return fNoCheck(fmt + 2, tail...);
        } else {
            appendNoCheck(fmt[0]);
            return fNoCheck(fmt + 1, head, tail...);
        }
    }

    void fNoCheck(const char* fmt) {
        appendNoCheck(fmt);
        assert(len < allocLen);
        s[len] = '\0';
    }

    template <typename T>
    void appendNoCheck(const T& what) {
        len += FStrUtil::appendNoCheck(s + len, what);
        assert(len <= allocLen);
    }

    template <typename... Types>
    static int charsNeeded(const Types&... args) {
        return sum(FStrUtil::charsNeeded(args)...);
    }

    char* s;
    int len;
    int allocLen;
};
