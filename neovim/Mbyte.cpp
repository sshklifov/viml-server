#include "Mbyte.hpp"
#include "Ascii.hpp"

#include <cinttypes>
#include <cstdlib>
#include <cstring>

// Like strcpy() but allows overlapped source and destination.
#define STRMOVE(d, s)       memmove((d), (s), strlen(s) + 1)

// To speed up BYTELEN(); keep a lookup table to quickly get the length in
// bytes of a UTF-8 character from the first byte of a UTF-8 string.  Bytes
// which are illegal when used as the first byte have a 1.  The NUL byte has
// length 1.
const uint8_t utf8len_tab[] = {
    // ?1 ?2 ?3 ?4 ?5 ?6 ?7 ?8 ?9 ?A ?B ?C ?D ?E ?F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 0?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 1?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 2?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 3?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 4?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 5?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 6?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 7?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 8?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 9?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // A?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // B?
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // C?
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // D?
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // E?
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1,  // F?
};

// Like utf8len_tab above, but using a zero for illegal lead bytes.
const uint8_t utf8len_tab_zero[] = {
    // ?1 ?2 ?3 ?4 ?5 ?6 ?7 ?8 ?9 ?A ?B ?C ?D ?E ?F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 0?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 1?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 2?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 3?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 4?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 5?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 6?
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 7?
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8?
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9?
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A?
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B?
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // C?
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // D?
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // E?
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 0, 0,  // F?
};

// Canonical encoding names and their properties.
// "iso-8859-n" is handled by enc_canonize() directly.
const char* enc_canon_table[] = {
    "latin1",
    "iso-8859-2",
    "iso-8859-3",
    "iso-8859-4",
    "iso-8859-5",
    "iso-8859-6",
    "iso-8859-7",
    "iso-8859-8",
    "iso-8859-9",
    "iso-8859-10",
    "iso-8859-11",
    "iso-8859-13",
    "iso-8859-14",
    "iso-8859-15",
    "koi8-r",
    "koi8-u",
    "utf-8",
    "ucs-2",
    "ucs-2le",
    "utf-16",
    "utf-16le",
    "ucs-4",
    "ucs-4le",

    // For debugging DBCS encoding on Unix.
    "debug",
    "euc-jp",
    "sjis",
    "euc-kr",
    "euc-cn",
    "euc-tw",
    "big5",

    // MS-DOS and MS-Windows codepages are included here, so that they can be
    // used on Unix too.  Most of them are similar to ISO-8859 encodings, but
    // not exactly the same.
    "cp437",   // like iso-8859-1
    "cp737",   // like iso-8859-7
    "cp775",   // Baltic
    "cp850",   // like iso-8859-4
    "cp852",   // like iso-8859-1
    "cp855",   // like iso-8859-2
    "cp857",   // like iso-8859-5
    "cp860",   // like iso-8859-9
    "cp861",   // like iso-8859-1
    "cp862",   // like iso-8859-1
    "cp863",   // like iso-8859-8
    "cp865",   // like iso-8859-1
    "cp866",   // like iso-8859-5
    "cp869",   // like iso-8859-7
    "cp874",   // Thai
    "cp932",
    "cp936",
    "cp949",
    "cp950",
    "cp1250",   // Czech, Polish, etc.
    "cp1251",   // Cyrillic
    // cp1252 is considered to be equal to latin1
    "cp1253",   // Greek
    "cp1254",   // Turkish
    "cp1255",   // Hebrew
    "cp1256",   // Arabic
    "cp1257",   // Baltic
    "cp1258",   // Vietnamese

    "macroman",      // Mac OS
    "hp-roman8",       // HP Roman8
    NULL
};

// Aliases for encoding names.
const char* enc_alias_table[] = {
    "ansi",
    "iso-8859-1",
    "latin2",
    "latin3",
    "latin4",
    "cyrillic",
    "arabic",
    "greek",
    "hebrew",
    "latin5",
    "turkish",
    "latin6",
    "nordic",
    "thai",
    "latin7",
    "latin8",
    "latin9",
    "utf8",
    "unicode",
    "ucs2",
    "ucs2be",
    "ucs-2be",
    "ucs2le",
    "utf16",
    "utf16be",
    "utf-16be",
    "utf16le",
    "ucs4",
    "ucs4be",
    "ucs-4be",
    "ucs4le",
    "utf32",
    "utf-32",
    "utf32be",
    "utf-32be",
    "utf32le",
    "utf-32le",
    "932",
    "949",
    "936",
    "gbk",
    "950",
    "eucjp",
    "unix-jis",
    "ujis",
    "shift-jis",
    "pck",        // Sun: PCK
    "euckr",
    "5601",       // Sun: KS C 5601
    "euccn",
    "gb2312",
    "euctw",
    "japan",
    "korea",
    "prc",
    "zh-cn",
    "chinese",
    "zh-tw",
    "taiwan",
    "cp950",
    "950",
    "mac",
    "mac-roman",
    NULL
};

/// Find encoding "name" in the list of canonical encoding names.
/// Returns -1 if not found.
int enc_canon_search(const char* name) {
    for (int i = 0; enc_canon_table[i] != NULL; i++) {
        if (strcmp(name, enc_canon_table[i]) == 0) {
            return i;
        }
    }
    return -1;
}

/// Search for an encoding alias of "name".
/// Returns -1 when not found.
int enc_alias_search(const char* name) {
    for (int i = 0; enc_alias_table[i] != NULL; i++) {
        if (strcmp(name, enc_alias_table[i]) == 0) {
            return i;
        }
    }
    return -1;
}

bool valid_encoding_len(const char *enc, int len) {
    if (strcmp(enc, "default") == 0) {
        return true;
    }

    // copy "enc" to allocated memory, with room for two '-'
    char* r = (char*)malloc(len + 3);
    // Make it all lower case and replace '_' with '-'.
    char* p = r;
    for (int i = 0; i < len; ++i) {
        if (enc[len] == '_') {
            *p++ = '-';
        } else {
            *p++ = TOLOWER_ASC(enc[len]);
        }
    }
    *p = NUL;

    // Skip "2byte-" and "8bit-".
    if (strncmp(r, "2byte-", 6) == 0) {
        p = r + 6;
    }
    if (strncmp(p, "8bit-", 5) == 0) {
        p = r + 5;
    }

    // Change "microsoft-cp" to "cp".  Used in some spell files.
    if (strncmp(p, "microsoft-cp", 12) == 0) {
        STRMOVE(p, p + 10);
    }

    // "iso8859" -> "iso-8859"
    if (strncmp(p, "iso8859", 7) == 0) {
        STRMOVE(p + 4, p + 3);
        p[3] = '-';
    }

    // "iso-8859n" -> "iso-8859-n"
    if (strncmp(p, "iso-8859", 8) == 0 && p[8] != '-') {
        STRMOVE(p + 9, p + 8);
        p[8] = '-';
    }

    // "latin-N" -> "latinN"
    if (strncmp(p, "latin-", 6) == 0) {
        STRMOVE(p + 5, p + 6);
    }

    if (enc_canon_search(p) >= 0) {
        free(r);
        return true;
    } else if (enc_alias_search(p) >= 0) {
        free(r);
        return true;
    }
    free(r);
    return false;
}

