#pragma once

#include <cstring>
#include <cassert>

struct StringView {
    StringView();
    explicit StringView(const char* s);
    StringView(const char* begin, const char* end);
    StringView(const char* s, int n);

    bool empty() const;
    int length() const;

    char operator[](int idx) const;

    char left() const;
    char right() const;

    bool beginsWith(char c) const;
    bool endsWith(char c) const;
    bool beginsWith(const char* prefix) const;

    const char* find(char c) const;
    const char* find(const char* it, char c) const;
    const char* find(char c1, char c2) const;

    StringView trunc(int newSize) const;

    StringView popLeft(int count = 1) const;
    StringView popRight(int count = 1) const;

    StringView trimLineFeed() const;
    StringView trimLeftSpace() const;
    StringView trimRightSpace() const;
    StringView trimSpace() const;

    StringView popLineFeed() const;

    int cmp(StringView other) const;
    bool operator<(const StringView& other) const;
    bool operator>(const StringView& other) const;
    bool operator<=(const StringView& other) const;
    bool operator>=(const StringView& other) const;
    bool operator==(const StringView& other) const;
    bool operator!=(const StringView& other) const;

    const char* begin;
    const char* end;
};
