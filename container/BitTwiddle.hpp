#pragma once

#include <cassert>
#include <cstdint>

// Undefined result for x = 0
unsigned logOfTwo(unsigned x);
unsigned logOfTwo(uint64_t x);

// Undefined result for x = 0
unsigned logOfTen(unsigned x);

// Do not call with x = 0!
unsigned prevPowerOfTen(unsigned x);

unsigned powerTen(unsigned x);
