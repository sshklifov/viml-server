#pragma once

#include "FStrUtil.hpp"
/* #include <cstring> */

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
    FStr(const FStr& rhs) = delete;
    FStr(FStr&& rhs);

    FStr& operator=(FStr&& rhs);

    void append(const char* s);

    template <typename... Types>
    void appendf(const char* fmt, Types... args) {
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
    void fNoCheck(const char* fmt, T head, Types... tail) {
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
    void appendNoCheck(T what) {
        len += FStrUtil::appendNoCheck(s + len, what);
        assert(len <= allocLen);
    }

    template <typename... Types>
    static int charsNeeded(Types... args) {
        return sum(FStrUtil::charsNeeded(args)...);
    }

    char* s;
    int len;
    int allocLen;
};
