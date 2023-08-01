#pragma once

#include <StringView.hpp>

struct LineView : public StringView {
    using StringView::StringView;

    int popSpaces() {
        const char* oldBegin = begin;
        while (begin < end) {
            if (*begin != ' ' && *begin != '\t') {
                break;
            }
            ++begin;
        }
        return begin - oldBegin;
    }

    LineView copy() {
        return LineView(*this);
    }

    StringView popWord() {
        popSpaces();
        const char* oldBegin = begin;
        while (begin < end) {
            if (*begin == ' ' || *begin == '\t' || *begin == '\n') {
                break;
            }
            ++begin;
        }
        return StringView(oldBegin, begin);
    }

    LineView& dropSpaces() {
        popSpaces();
        return (*this);
    }
};

struct ProgramView : public StringView {
    using StringView::StringView;

    LineView popLine() {
        const char* oldBegin = begin;
        while (begin < end) {
            if (*begin == '\n') {
                ++begin;
                break;
            }
            ++begin;
        }
        LineView res(oldBegin, begin);
        poppedLines += !res.empty();
        return res;
    }

    int poppedLines = 0;
};
