#pragma ocne

#include <cstddef>

/// Check that "c" is a normal identifier character:
/// Letters and characters from the 'isident' option.
///
/// @param  c  character to check
bool vim_isIDc(int c);

/// Check that "c" is a keyword character:
/// Letters and characters from 'iskeyword' option for the current buffer.
/// For multi-byte characters mb_get_class() is used (builtin rules).
///
/// @param  c  character to check
bool vim_iswordc(const int c);

/// Like `skipwhite`, but skip up to `len` characters.
/// @see skipwhite
///
/// @param[in]  p    String to skip in.
/// @param[in]  len  Max length to skip.
///
/// @return Pointer to character after the skipped whitespace, or the `len`-th
///         character in the string.
const char *skipwhite_len(const char *p, std::size_t len);

/// skipwhite: skip over ' ' and '\t'.
///
/// @param[in]  p  String to skip in.
///
/// @return Pointer to character after the skipped whitespace.
const char *skipwhite(const char *const p);

/// Skip over digits
///
/// @param[in]  q  String to skip digits in.
///
/// @return Pointer to the character after the skipped digits.
const char *skipdigits(const char *q);
