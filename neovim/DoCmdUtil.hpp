#pragma once

#include "Ascii.hpp"
#include "Charset.hpp"
#include "Message.hpp"
#include "OptionDefs.hpp"
#include "ExCmdsDefs.hpp"

#include <cstring>

/// Check for class item. "pp" points to the '['.
// Returns a pointer to after the item.
// If no item is found, skip the '['.
static const char* skip_class(const char* pp) {
    /// Check for a character class name "[:name:]".
    const char* class_names[] = {
        "alnum:]",
        "alpha:]",
        "blank:]",
        "cntrl:]",
        "digit:]",
        "graph:]",
        "lower:]",
        "print:]",
        "punct:]",
        "space:]",
        "upper:]",
        "xdigit:]",
        "tab:]",
        "return:]",
        "backspace:]",
        "escape:]",
        "ident:]",
        "keyword:]",
        "fname:]",
    };
    if (pp[1] == ':') {
        for (int i = 0; i < 19; i++) {
            int n = strlen(class_names[i]);
            if (strncmp(pp + 2, class_names[i], n) == 0) {
                return pp + n + 2;
            }
        }
    }

    // Check for an equivalence class name "[=a=]".
    if (pp[1] == '=' && pp[2] != NUL) {
        if (pp[3] == '=' && pp[4] == ']')
        return pp += 5;
    }

    // Check for a collating element "[.a.]".  "pp" points to the '['.
    if (pp[1] == '.' && pp[2] != NUL) {
        if (pp[3] == '.' && pp[4] == ']') {
            return pp + 5;
        }
    }

    return pp + 1;
}

/// Skip over a "[]" range.
/// "p" must point to the character after the '['.
static const char* skip_anyof(const char* p) {
    // REGEXP_INRANGE contains all characters which are always special in a []
    // range after '\'.
    // REGEXP_ABBR contains all characters which act as abbreviations after '\'.
    // These are:
    //  \n  - New line (NL).
    //  \r  - Carriage Return (CR).
    //  \t  - Tab (TAB).
    //  \e  - Escape (ESC).
    //  \b  - Backspace (Ctrl_H).
    //  \d  - Character code in decimal, eg \d123
    //  \o  - Character code in octal, eg \o80
    //  \x  - Character code in hex, eg \x4a
    //  \u  - Multibyte character code, eg \u20ac
    //  \U  - Long multibyte character code, eg \U12345678
    char REGEXP_INRANGE[] = "]^-n\\";
    char REGEXP_ABBR[] = "nrtebdoxuU";

    if (*p == '^') {  // Complement of range.
        p++;
    }
    if (*p == ']' || *p == '-') {
        p++;
    }
    while (*p != NUL && *p != ']') {
        if (*p == '-') {
            p++;
            if (*p != ']' && *p != NUL) {
                p++;
            }
        } else if (*p == '\\'
            && (strchr(REGEXP_INRANGE, p[1]) != NULL
                || (!reg_cpo_lit && strchr(REGEXP_ABBR, p[1]) != NULL))) {
            p += 2;
        } else if (*p == '[') {
            p = skip_class(p);
        } else {
            p++;
        }
    }

    if (*p == NUL) {
        throw msg("Expecting ]", p);
    }
    return p;
}

/// Skip past regular expression.
/// Stop at end of "startp" or where "delim" is found ('/', '?', etc).
/// Take care of characters with a backslash in front of it.
/// Skip strings inside [ and ].
static const char* skip_regexp(const char* p, int delim) {
    magic_T mymagic;
    if (magic) {
        mymagic = MAGIC_ON;
    } else {
        mymagic = MAGIC_ON;
    }

    while (*p != NUL && *p != delim) {
        if ((p[0] == '[' && mymagic >= MAGIC_ON) || (p[0] == '\\' && p[1] == '[' && mymagic <= MAGIC_OFF)) {
            p = skip_anyof(p + 1);
        } else if (p[0] == '\\' && p[1] != NUL) {
            p++; // skip next character
            if (*p == 'v') {
                mymagic = MAGIC_ALL;
            } else if (*p == 'V') {
                mymagic = MAGIC_NONE;
            }
        }
        p++;
    }
    return p;
}

/// Call skip_regexp() and when the delimiter does not match give an error
static const char* skip_regexp_err(const char *startp, int delim) {
    const char* p = skip_regexp(startp, delim);
    if (*p != delim) {
        throw msg(p, "Missing delimiter after search pattern");
    }
    return p;
}

/// Check for an Ex command with optional tail.
/// If there is a match advance "pp" to the argument and return true.
///
/// @param pp   start of command
/// @param cmd  name of command
/// @param len  required length
static bool checkforcmd(const char*& pp, const char* cmd, int len) {
    int i;
    for (i = 0; cmd[i] != NUL; i++) {
        if (cmd[i] != pp[i]) {
            break;
        }
    }
    if (i >= len && !ASCII_ISALPHA(pp[i])) {
        pp = skipwhite(pp + i);
        return true;
    }
    return false;
}

/// Check if "c" is:
/// - end of command
/// - comment
// - command separator
static int ends_excmd(int c) {
    return c == '"' || c == NUL || c == '|' || c == '\n';
}

static int ends_notrlcom(int c) {
    return c == NUL || c == '|' || c == '\n';
}
