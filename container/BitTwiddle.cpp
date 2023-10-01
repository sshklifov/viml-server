#include "BitTwiddle.hpp"

#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n

unsigned char tableLogTwo[256] = {
    0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
    LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7),
};

#undef LT

const unsigned tablePowTen[] = {
    1,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000,
};

unsigned logOfTwo(unsigned x) {

    unsigned tt = x >> 16;
    unsigned r;
    if (tt) {
        unsigned t = tt >> 8;
        r = t ? 24u + tableLogTwo[t] : 16 + tableLogTwo[tt];
    } else {
        unsigned t = x >> 8;
        r = t ? 8 + tableLogTwo[t] : tableLogTwo[x];
    }
#ifndef NDEBUG
    if (x != 0) {
        assert((1u << r) <= x);
        assert(r == 31 || (1u << (r + 1)) > x);
    }
#endif
    return r;
}

unsigned logOfTwo(uint64_t x) {
    unsigned tt = x >> 32;
    if (tt) {
        return 32 + logOfTwo(tt);
    } else {
        return logOfTwo(x);
    }
}

unsigned logOfTen(unsigned x) {
    unsigned t = ((logOfTwo(x) + 1) * 1233) >> 12;
    unsigned r = t - (x < tablePowTen[t]);

#ifndef NDEBUG
    if (x != 0) {
        assert(tablePowTen[r] <= x);
        assert(r == 9 || tablePowTen[r + 1] > x);
    }
#endif
    return r;
}

unsigned prevPowerOfTen(unsigned x) {
    assert(x != 0);
    unsigned t = logOfTen(x);
    return tablePowTen[t];
}

unsigned powerTen(unsigned x) {
    assert(x <= 9);
    return tablePowTen[x];
}
