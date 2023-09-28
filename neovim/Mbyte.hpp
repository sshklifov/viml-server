#pragma once

#include <cstring>

/// Find the canonical name for encoding "enc".
/// @return True if the encoding is valid
bool valid_encoding_len(const char *enc, int len);
