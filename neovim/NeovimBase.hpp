#pragma once

#include <Ascii.hpp>
#include <Charset.hpp>
#include <Config.hpp>
#include <Message.hpp>

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

/// types for expressions.
typedef enum {
    EXPR_UNKNOWN = 0,
    EXPR_EQUAL,         ///< ==
    EXPR_NEQUAL,        ///< !=
    EXPR_GREATER,       ///< >
    EXPR_GEQUAL,        ///< >=
    EXPR_SMALLER,       ///< <
    EXPR_SEQUAL,        ///< <=
    EXPR_MATCH,         ///< =~
    EXPR_NOMATCH,       ///< !~
    EXPR_IS,            ///< is
    EXPR_ISNOT,         ///< isnot
} exprtype_T;

/// case for expressions
typedef enum {
    CASE_IGNORE,
    CASE_MATCH,
    CASE_OPTION,
} exprcase_T;

/// Magicness of a pattern, used by regexp code.
/// The order and values matter:
///  magic <= MAGIC_OFF includes MAGIC_NONE
///  magic >= MAGIC_ON  includes MAGIC_ALL
typedef enum {
  MAGIC_NONE = 1,  ///< "\V" very unmagic
  MAGIC_OFF = 2,   ///< "\M" or 'magic' off
  MAGIC_ON = 3,    ///< "\m" or 'magic'
  MAGIC_ALL = 4,   ///< "\v" very magic
} magic_T;
