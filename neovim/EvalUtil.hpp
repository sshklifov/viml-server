#pragma once

#include "Ascii.hpp"
#include "Charset.hpp"
#include "Message.hpp"

#include <cstring>

/// VimL variable types, for use in typval_T.v_type
typedef enum {
  VAR_UNKNOWN = 0,  ///< Unknown (unspecified) value.
  VAR_NUMBER,       ///< Number, .v_number is used.
  VAR_STRING,       ///< String, .v_string is used.
  VAR_FUNC,         ///< Function reference, .v_string is used as function name.
  VAR_LIST,         ///< List, .v_list is used.
  VAR_DICT,         ///< Dictionary, .v_dict is used.
  VAR_FLOAT,        ///< Floating-point value, .v_float is used.
  VAR_BOOL,         ///< true, false
  VAR_SPECIAL,      ///< Special value (null), .v_special is used.
  VAR_PARTIAL,      ///< Partial, .v_partial is used.
  VAR_BLOB,         ///< Blob, .v_blob is used.
} VarType;

/// @return  whether `regname` is a valid name of a yank register.
///
/// @note: There is no check for 0 (default register), caller should do this.
/// The black hole register '_' is regarded as valid.
///
/// @param regname name of register
/// @param writing allow only writable registers
static bool valid_yank_reg(int regname, bool writing = false) {
    if ((regname > 0 && isalnum(regname))
        || (!writing && ascii_haschar("/.%:=", regname))
        || regname == '#'
        || regname == '"'
        || regname == '-'
        || regname == '_'
        || regname == '*'
        || regname == '+') {
        return true;
    }
    return false;
}

/// Get the length of an environment variable name.
/// @return  0 for error.
static int get_env_len(const char* arg) {
    const char* p = arg;
    while (vim_isIDc(*p)) {
        ++p;
    }
    return p - arg;
}

/// Skip over the scope of an option: "&", "&g:" or "&l:".
///
/// @param arg  points to the "&" or '+' when called, to "option" when returning.
static const char *skip_option_scope(const char *arg) {
    arg++;
    if (*arg == 'g' && arg[1] == ':') {
        arg += 2;
    } else if (*arg == 'l' && arg[1] == ':') {
        arg += 2;
    }
    return arg;
}

/// Get the length of an option name.
/// 
/// @param arg  points to the name part of the option.
/// @return  0 for error.
static int get_option_len(const char *arg) {
    if (arg[0] == 't' && arg[1] == '_' && arg[2] != NUL && arg[3] != NUL) {
        return 4; // t_xx/termcap option
    } else {
        int len = 0;
        while (ASCII_ISALPHA(arg[len])) {
            len++;
        }
        return len;
    }
}

/// Skip over a 'str''ing' constant
/// @return NULL for error.
static const char* skip_lit_string(const char* arg) {
    // Find the end of the string, skipping ''.
    for (arg = arg + 1; *arg != NUL; arg++) {
        if (*arg == '\'') {
            if (arg[1] != '\'') {
                return arg + 1;
            }
            arg++;
        }
    }
    throw msg(arg, "Missing closing quote");
}

/// Skip over a string constant
/// @return NULL for error.
static const char* skip_string(const char* arg) {
    // Find the end of the string, skipping backslashed characters.
    for (arg = arg + 1; *arg != NUL; arg++) {
        if (*arg == '\\') {
            ++arg;
        } else if (*arg == '"') {
            return arg + 1;
        }
    }
    throw msg(arg, "Missing closing quote");
}

/// Skip a "0z" blob constant.
static const char* skip_blob(const char *arg) {
    // Blob constant: 0z0123456789abcdef
    arg += 2;
    for (arg = arg + 2; ascii_isxdigit(*arg); arg += 2) {
        if (!ascii_isxdigit(arg[1])) {
            throw msg(arg, "Blob literal should have an even number of hex characters");
        }
        if (arg[2] == '.' && ascii_isxdigit(arg[3])) {
            arg++;
        }
    }
    return arg;
}

/// Skip a number constant.
///
/// @return  NULL for error
static const char* skip_number(const char *arg, int& maybe_float) {
    maybe_float = 0;
    bool (*is_digit)(int);
    if (arg[0] == '0' && tolower(arg[1]) == 'x') {
        // Detect hexadecimal: 0x or 0X followed by hex digit.
        arg += 2;
        is_digit = ascii_isxdigit;
    } else if (arg[0] == '0' && tolower(arg[1]) == 'b') {
        // Detect binary: 0b or 0B followed by 0 or 1.
        arg += 2;
        is_digit = ascii_isbdigit;
    } else if (arg[0] == '0' && tolower(arg[1]) == 'o') {
        // Detect octal: 0o or 0O followed by octal digits (without '8' or '9').
        arg += 2;
        is_digit = ascii_isodigit;
    } else {
        // Detect old octal format: 0 followed by octal digits.
        // Or float or decimal, doesn't matter which it is.
        is_digit = ascii_isdigit;
        maybe_float = 1;
    }

    while (is_digit(*arg)) {
        ++arg;
    }
    // Check for an alphanumeric character immediately following, that is
    // most likely a typo.
    if (ASCII_ISALNUM(*arg)) {
        throw msg(arg, "Invalid Number");
    }
    return arg;
}

/// Skip a Number, Float or Blob
static const char* skip_numerical(const char* arg, int& type) {
    if (*arg == '0' && (arg[1] == 'z' || arg[1] == 'Z')) {
        type = VAR_BLOB;
        return skip_blob(arg);
    } else {
        // float, decimal, binary, hex or octal number
        int maybe_float;
        arg = skip_number(arg, maybe_float);
        // We accept a float when the format matches
        // "[0-9]\+\.[0-9]\+\([eE][+-]\?[0-9]\+\)\?".  This is very
        // strict to avoid backwards compatibility problems.
        // Don't look for a float after the "." operator, so that
        // ":let vers = 1.2.3" doesn't fail.
        if (maybe_float && *arg == '.' && ascii_isdigit(arg[1])) {
            const char* p = skipdigits(arg + 2);
            if (tolower(*p) == 'e') {
                p++;
                if (*p == '-' || *p == '+') {
                    p++;
                }
                if (!ascii_isdigit(*p)) {
                    // Was number, not float
                    type = VAR_NUMBER;
                    return arg;
                } else {
                    p = skipdigits(p + 1);
                }
            }
            if (ASCII_ISALPHA(*p) || *p == '.') {
                // Was number, not float
                type = VAR_NUMBER;
                return arg;
            } else {
                type = VAR_FLOAT;
                return p;
            }
        } else {
            type = VAR_NUMBER;
            return arg;
        }
    }
}

/// Get the key length for #{key: val}.
///
/// @return  0 when there is no valid key.
static int get_literal_key_len(const char* arg) {
    const char* p = arg;
    while (ASCII_ISALNUM(*p) || *p == '_' || *p == '-') {
        ++p;
    }
    return p - arg;
}

/// @return  true if character "c" can be used in a variable or function name.
///                  Does not include '{' or '}' for magic braces.
static bool eval_isnamec(int c) {
    const char AUTOLOAD_CHAR = '#';
    return ASCII_ISALNUM(c) || c == '_' || c == ':' || c == AUTOLOAD_CHAR;
}

/// @return  true if character "c" can be used as the first character in a
///                  variable or function name (excluding '{' and '}').
static bool eval_isnamec1(int c) {
    return ASCII_ISALPHA(c) || c == '_';
}

/// @return  true if character "c" can be used as a character of a dictionary
//                   key with the dot operator.
static bool eval_isdictc(int c) {
    return ASCII_ISALNUM(c) || c == '_';
}

/// Skips one character past the end of the name of a v:lua function.
/// @param p    Pointer to the char AFTER the "v:lua." prefix.
/// @return Pointer to the char one past the end of the function's name.
static int get_luafunc_name_len(const char* arg) {
    const char* p = arg;
    while (ASCII_ISALNUM(*p) || *p == '_' || *p == '-' || *p == '.' || *p == '\'') {
        p++;
    }
    return p - arg;
}

/// Skips one character past the end of the dictionary key.
static int get_dict_key_len(const char* arg) {
    const char* p = arg;
    while (eval_isdictc(*p)) {
        ++p;
    }
    return p - arg;
}

/// @return  5 if "p" starts with "<SID>" or "<SNR>" (ignoring case).
///          2 if "p" starts with "s:".
///          0 otherwise.
static int get_fname_script_len(const char* p) {
    if (p[0] == '<' && tolower(p[1]) == 's') {
        if (tolower(p[2]) == 'i' && tolower(p[3]) == 'd' && p[4] == '>') {
            return 5;
        }
        if (tolower(p[2]) == 'n' && tolower(p[3]) == 'r' && p[4] == '>') {
            return 5;
        }
    }
    if (p[0] == 's' && p[1] == ':') {
        return 2;
    }
    return 0;
}

/// Get the length of the name of a function or internal variable.
///
/// @param arg          is advanced to the first non-white character after the name.
/// @param allow_scope  name can be preceded by a scope
///
/// @return  0 if something is wrong.
static int get_id_len(const char* arg, int allow_scope) {
    const char* p = arg;
    if (allow_scope) {
        int len = get_fname_script_len(p);
        if (len > 0) {
            // literal "<SID>", "s:" or "<SNR>"
            p += len;
        }
    }
    if (eval_isnamec1(*p)) {
        p++;
        // Find the end of the name.
        for (; eval_isnamec(*p); p++) {
            if (*p == ':') {
                if (!allow_scope) {
                    break;
                }
                const char* namespace_char = "abglstvw";
                // "s:" is start of "s:var", but "n:" is not and can be used in
                // slice "[n:]". Also "xx:" is not a namespace.
                int len = p - arg;
                if (len > 1 || (len == 1 && !ascii_haschar(namespace_char, *arg))) {
                    break;
                }
            }
        }
    }
    return p - arg;
}
