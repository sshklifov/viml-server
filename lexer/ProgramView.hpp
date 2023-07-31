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
        return LineView(oldBegin, begin);
    }

    LineView getLine() const {
        ProgramView copy(*this);
        return copy.popLine();
    }
};
