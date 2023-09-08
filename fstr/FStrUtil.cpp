#include "FStrUtil.hpp"
#include "BitTwiddle.hpp"

#include <FStr.hpp>
#include <StringView.hpp>

#include <cstring>

namespace FStrUtil {
    template<>
    int charsNeeded(const char* s) {
        return strlen(s);
    }

    template<>
    int appendNoCheck(char* dest, const char* source) {
        int len = 0;
        while (source[len]) {
            dest[len] = source[len];
            ++len;
        }
        return len;
    }

    template<>
    int charsNeeded(char c) {
        return 1;
    }

    template<>
    int appendNoCheck(char* dest, char c) {
        *dest = c;
        return 1;
    }

    template<>
    int charsNeeded(unsigned n) {
        return n > 0 ? logOfTen(n) + 1 : 1;
    }

    template<>
    int appendNoCheck(char* dest, unsigned u) {
        int len = 0;
        int divBase = u > 0 ? prevPowerOfTen(u) : u;
        while (u > 9) {
            unsigned dig = u / divBase;
            char c = '0' + dig;
            len += appendNoCheck(dest, c);
            u -= divBase * dig;
            divBase /= 10;
        }
        char c = '0' + u;
        len += appendNoCheck(dest, c);
        return len;
    }

    template<>
    int charsNeeded(int n) {
        if (n < 0) {
            return 2 + logOfTen(-n);
        } else if (n > 0) {
            return 1 + logOfTen(n);
        } else {
            return 1;
        }
    }

    template<>
    int appendNoCheck(char* dest, int n) {
        int len = 0;
        if (n < 0) {
            len += appendNoCheck(dest, '-');
            len += appendNoCheck(dest, static_cast<unsigned>(-n));
            return len;
        } else {
            len += appendNoCheck(dest, static_cast<unsigned>(n));
            return len;
        }
    }

    template<>
    int charsNeeded(const FStr& what) {
        return what.length();
    }

    template <>
    int appendNoCheck(char* dest, const FStr& what) {
        return appendNoCheck(dest, what.str());
    }

    template<>
    int charsNeeded(const StringView& what) {
        return what.length();
    }

    template <>
    int appendNoCheck(char* dest, const StringView& what) {
        strncpy(dest, what.begin, what.length());
        return what.length();
    }
}
