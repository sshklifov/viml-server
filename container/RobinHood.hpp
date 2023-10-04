//                 ______  _____                 ______                _________
//  ______________ ___  /_ ___(_)_______         ___  /_ ______ ______ ______  /
//  __  ___/_  __ \__  __ \__  / __  __ \        __  __ \_  __ \_  __ \_  __  /
//  _  /    / /_/ /_  /_/ /_  /  _  / / /        _  / / // /_/ // /_/ // /_/ /
//  /_/     \____/ /_.___/ /_/   /_/ /_/ ________/_/ /_/ \____/ \____/ \__,_/
//                                      _/_____/
//
// Fast & memory efficient hashtable based on robin hood hashing for C++11/14/17/20
// https://github.com/martinus/robin-hood-hashing
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2021 Martin Ankerl <http://martin.ankerl.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

// fallthrough
#ifndef __has_cpp_attribute // For backwards compatibility
#    define __has_cpp_attribute(x) 0
#endif
#if __has_cpp_attribute(clang::fallthrough)
#    define ROBIN_HOOD_FALLTHROUGH() [[clang::fallthrough]]
#elif __has_cpp_attribute(gnu::fallthrough)
#    define ROBIN_HOOD_FALLTHROUGH() [[gnu::fallthrough]]
#else
#    define ROBIN_HOOD_FALLTHROUGH()
#endif

template <typename T>
inline T unaligned_load(void const* ptr) noexcept {
    // using memcpy so we don't get into unaligned load problems.
    // compiler should optimize this very well anyways.
    T t;
    std::memcpy(&t, ptr, sizeof(T));
    return t;
}

inline uint64_t hash_bytes(void const* ptr, size_t len) noexcept {
    static constexpr uint64_t m = UINT64_C(0xc6a4a7935bd1e995);
    static constexpr uint64_t seed = UINT64_C(0xe17a1465);
    static constexpr unsigned int r = 47;

    auto const* const data64 = static_cast<uint64_t const*>(ptr);
    uint64_t h = seed ^ (len * m);

    size_t const n_blocks = len / 8;
    for (size_t i = 0; i < n_blocks; ++i) {
        auto k = unaligned_load<uint64_t>(data64 + i);

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    auto const* const data8 = reinterpret_cast<uint8_t const*>(data64 + n_blocks);
    switch (len & 7U) {
    case 7:
        h ^= static_cast<uint64_t>(data8[6]) << 48U;
        ROBIN_HOOD_FALLTHROUGH(); // FALLTHROUGH
    case 6:
        h ^= static_cast<uint64_t>(data8[5]) << 40U;
        ROBIN_HOOD_FALLTHROUGH(); // FALLTHROUGH
    case 5:
        h ^= static_cast<uint64_t>(data8[4]) << 32U;
        ROBIN_HOOD_FALLTHROUGH(); // FALLTHROUGH
    case 4:
        h ^= static_cast<uint64_t>(data8[3]) << 24U;
        ROBIN_HOOD_FALLTHROUGH(); // FALLTHROUGH
    case 3:
        h ^= static_cast<uint64_t>(data8[2]) << 16U;
        ROBIN_HOOD_FALLTHROUGH(); // FALLTHROUGH
    case 2:
        h ^= static_cast<uint64_t>(data8[1]) << 8U;
        ROBIN_HOOD_FALLTHROUGH(); // FALLTHROUGH
    case 1:
        h ^= static_cast<uint64_t>(data8[0]);
        h *= m;
        ROBIN_HOOD_FALLTHROUGH(); // FALLTHROUGH
    default:
        break;
    }

    h ^= h >> r;

    // not doing the final step here, because this will be done by keyToIdx anyways
    // h *= m;
    // h ^= h >> r;
    return h;
}
