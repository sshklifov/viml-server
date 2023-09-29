#pragma once

#include <type_traits>
#include <functional>
#include <cassert>

struct FStr {
    FStr();
    FStr(const char* s);
    FStr(const char* s, int n);
    FStr(const char* begin, const char* end);
    FStr(const FStr& rhs);
    FStr(FStr&& rhs);

    FStr& operator=(const FStr& rhs);
    FStr& operator=(FStr&& rhs);

    char operator[](int i) const;
    char& operator[](int i);

    void append(int d);
    void append(unsigned u);
    void append(char c);
    void append(const char* s);
    void append(const char* s, int n);
    void append(const FStr& other);

    template <typename T, typename std::enable_if<std::is_enum<T>::value, bool>::type = true>
    void append(T en) {
        append((int)en);
    }

    FStr& operator=(char c);
    FStr& operator=(const char* s);

    FStr& operator+=(char c);
    FStr& operator+=(const char* s);
    FStr& operator+=(const FStr& other);

    bool operator==(const char* other);
    bool operator!=(const char* other);
    bool operator==(const FStr& other);
    bool operator!=(const FStr& other);

    void replace(int begin, int end, const char* s);

    const char* str() const;

    int length() const;
    bool empty() const;

    template <typename T, typename... Types>
    void appendf(const char* fmt, const T& head, const Types&... tail) {
        int len = 0;
        while (fmt[len]) {
            if (fmt[len] == '{' && fmt[len + 1] == '}') {
                append(fmt, len);
                append(head);
                return appendf(fmt + len + 2, tail...);
            }
            ++len;
        }
        assert(false && "Too many arguments");
        s[len] = '\0';
        return;
    }

    void appendf(const char* fmt) {
        append(fmt);
    }

private:
    void allocAtLeast(int n);

    char* s;
    int len;
    int allocLen;
};

template <typename... Types>
FStr format(const char* fmt, const Types&... args) {
    FStr res;
    res.appendf(fmt, args...);
    return res;
}
