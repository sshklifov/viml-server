#pragma once

#include "Types.hpp"
#include "Pos.hpp"

#include <cinttypes>
#include <climits>

/// Type used for VimL VAR_NUMBER values
typedef int64_t varnumber_T;
typedef uint64_t uvarnumber_T;

/// Refcount for dict or list that should not be freed
enum { DO_NOT_FREE_CNT = (INT_MAX / 2), };

/// Additional values for tv_list_alloc() len argument
enum ListLenSpecials {
  /// List length is not known in advance
  ///
  /// To be used when there is neither a way to know how many elements will be
  /// needed nor are any educated guesses.
  kListLenUnknown = -1,
  /// List length *should* be known, but is actually not
  ///
  /// All occurrences of this value should be eventually removed. This is for
  /// the case when the only reason why list length is not known is that it
  /// would be hard to code without refactoring, but refactoring is needed.
  kListLenShouldKnow = -2,
  /// List length may be known in advance, but it requires too much effort
  ///
  /// To be used when it looks impractical to determine list length.
  kListLenMayKnow = -3,
};

/// Maximal possible value of varnumber_T variable
#define VARNUMBER_MAX INT64_MAX
#define UVARNUMBER_MAX UINT64_MAX

/// Minimal possible value of varnumber_T variable
#define VARNUMBER_MIN INT64_MIN

/// %d printf format specifier for varnumber_T
#define PRIdVARNUMBER PRId64

typedef struct listvar_S list_T;
typedef struct dictvar_S dict_T;
typedef struct partial_S partial_T;
typedef struct blobvar_S blob_T;

typedef struct ufunc ufunc_T;

/// Bool variable values
typedef enum {
  kBoolVarFalse,         ///< v:false
  kBoolVarTrue,          ///< v:true
} BoolVarValue;

/// Special variable values
typedef enum {
  kSpecialVarNull,   ///< v:null
} SpecialVarValue;

/// Variable lock status for typval_T.v_lock
typedef enum {
  VAR_UNLOCKED = 0,  ///< Not locked.
  VAR_LOCKED = 1,    ///< User lock, can be unlocked.
  VAR_FIXED = 2,     ///< Locked forever.
} VarLockStatus;

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

/// Structure that holds an internal variable value
typedef struct {
  VarType v_type;               ///< Variable type.
  VarLockStatus v_lock;         ///< Variable lock status.
  union typval_vval_union {
    varnumber_T v_number;       ///< Number, for VAR_NUMBER.
    BoolVarValue v_bool;        ///< Bool value, for VAR_BOOL
    SpecialVarValue v_special;  ///< Special value, for VAR_SPECIAL.
    float_T v_float;            ///< Floating-point number, for VAR_FLOAT.
    char *v_string;             ///< String, for VAR_STRING and VAR_FUNC, can be NULL.
    list_T *v_list;             ///< List for VAR_LIST, can be NULL.
    dict_T *v_dict;             ///< Dictionary for VAR_DICT, can be NULL.
    partial_T *v_partial;       ///< Closure: function with args.
    blob_T *v_blob;             ///< Blob for VAR_BLOB, can be NULL.
  } vval;                       ///< Actual value.
} typval_T;

#define TV_INITIAL_VALUE \
  ((typval_T) { \
    .v_type = VAR_UNKNOWN, \
    .v_lock = VAR_UNLOCKED, \
  })

/// Values for (struct dictvar_S).dv_scope
typedef enum {
  VAR_NO_SCOPE = 0,  ///< Not a scope dictionary.
  VAR_SCOPE = 1,  ///< Scope dictionary which requires prefix (a:, v:, …).
  VAR_DEF_SCOPE = 2,  ///< Scope dictionary which may be accessed without prefix
                      ///< (l:, g:).
} ScopeType;

/// Structure to hold an item of a list
typedef struct listitem_S listitem_T;

struct listitem_S {
  listitem_T *li_next;  ///< Next item in list.
  listitem_T *li_prev;  ///< Previous item in list.
  typval_T li_tv;  ///< Item value.
};

/// Structure used by those that are using an item in a list
typedef struct listwatch_S listwatch_T;

struct listwatch_S {
  listitem_T *lw_item;  ///< Item being watched.
  listwatch_T *lw_next;  ///< Next watcher.
};

/// Structure to hold info about a list
/// Order of members is optimized to reduce padding.
struct listvar_S {
  listitem_T *lv_first;  ///< First item, NULL if none.
  listitem_T *lv_last;  ///< Last item, NULL if none.
  listwatch_T *lv_watch;  ///< First watcher, NULL if none.
  listitem_T *lv_idx_item;  ///< When not NULL item at index "lv_idx".
  list_T *lv_copylist;  ///< Copied list used by deepcopy().
  list_T *lv_used_next;  ///< next list in used lists list.
  list_T *lv_used_prev;  ///< Previous list in used lists list.
  int lv_refcount;  ///< Reference count.
  int lv_len;  ///< Number of items.
  int lv_idx;  ///< Index of a cached item, used for optimising repeated l[idx].
  int lv_copyID;  ///< ID used by deepcopy().
  VarLockStatus lv_lock;  ///< Zero, VAR_LOCKED, VAR_FIXED.

  LuaRef lua_table_ref;
};

/// Static list with 10 items. Use tv_list_init_static10() to initialize.
typedef struct {
  list_T sl_list;  // must be first
  listitem_T sl_items[10];
} staticList10_T;

#define TV_LIST_STATIC10_INIT { \
  .sl_list = { \
  .lv_first = NULL, \
  .lv_last = NULL, \
  .lv_refcount = 0, \
  .lv_len = 0, \
  .lv_watch = NULL, \
  .lv_idx_item = NULL, \
  .lv_lock = VAR_FIXED, \
  .lv_used_next = NULL, \
  .lv_used_prev = NULL, \
  }, \
}

#define TV_DICTITEM_STRUCT(...) \
  struct { \
    typval_T di_tv;  /* Structure that holds scope dictionary itself. */ \
    uint8_t di_flags;  /* Flags. */ \
    char di_key[__VA_ARGS__];  /* Key value. */  /* NOLINT(runtime/arrays)*/ \
  }

/// Structure to hold a scope dictionary
///
/// @warning Must be compatible with dictitem_T.
///
/// For use in find_var_in_ht to pretend that it found dictionary item when it
/// finds scope dictionary.
typedef TV_DICTITEM_STRUCT(1) ScopeDictDictItem;

/// Structure to hold an item of a Dictionary
///
/// @warning Must be compatible with ScopeDictDictItem.
///
/// Also used for a variable.
typedef TV_DICTITEM_STRUCT() dictitem_T;

/// Flags for dictitem_T.di_flags
typedef enum {
  DI_FLAGS_RO = 1,  ///< Read-only value
  DI_FLAGS_RO_SBX = 2,  ///< Value, read-only in the sandbox
  DI_FLAGS_FIX = 4,  ///< Fixed value: cannot be :unlet or remove()d.
  DI_FLAGS_LOCK = 8,  ///< Locked value.
  DI_FLAGS_ALLOC = 16,  ///< Separately allocated.
} DictItemFlags;

/// Structure representing a Dictionary
struct dictvar_S {
    // TODO
};

/// Structure to hold info about a Blob
struct blobvar_S {
    // TODO
};

/// Type used for script ID
typedef int scid_T;
/// Format argument for scid_T
#define PRIdSCID "d"

/// SCript ConteXt (SCTX): identifies a script line.
/// When sourcing a script "sc_lnum" is zero, "sourcing_lnum" is the current
/// line number. When executing a user function "sc_lnum" is the line where the
/// function was defined, "sourcing_lnum" is the line number inside the
/// function.  When stored with a function, mapping, option, etc. "sc_lnum" is
/// the line number in the script "sc_sid".
typedef struct {
  scid_T sc_sid;     ///< script ID
  int sc_seq;        ///< sourcing sequence number
  linenr_T sc_lnum;  ///< line number
} sctx_T;

/// Maximum number of function arguments
enum { MAX_FUNC_ARGS = 20, };
/// Short variable name length
enum { VAR_SHORT_LEN = 20, };
/// Number of fixed variables used for arguments
enum { FIXVAR_CNT = 12, };

/// Structure to hold info for a function that is currently being executed.
typedef struct funccall_S funccall_T;

struct funccall_S {
    // TODO
};

/// Structure to hold info for a user function.
struct ufunc {
    // TODO
};

struct partial_S {
  int pt_refcount;    ///< Reference count.
  char *pt_name;      ///< Function name; when NULL use pt_func->name.
  ufunc_T *pt_func;   ///< Function pointer; when NULL lookup function with pt_name.
  bool pt_auto;       ///< When true the partial was created by using dict.member
                      ///< in handle_subscript().
  int pt_argc;        ///< Number of arguments.
  typval_T *pt_argv;  ///< Arguments in allocated array.
  dict_T *pt_dict;    ///< Dict for "self".
};

/// Structure used for explicit stack while garbage collecting hash tables
typedef struct ht_stack_S {
    // TODO
} ht_stack_T;

/// Structure used for explicit stack while garbage collecting lists
typedef struct list_stack_S {
  list_T *list;
  struct list_stack_S *prev;
} list_stack_T;

/// Structure representing one list item, used for sort array.
typedef struct {
  listitem_T *item;  ///< Sorted list item.
  int idx;  ///< Sorted list item index.
} ListSortItem;

typedef int (*ListSorter)(const void *, const void *);

#ifdef LOG_LIST_ACTIONS
/// List actions log entry
typedef struct {
  uintptr_t l;  ///< List log entry belongs to.
  uintptr_t li1;  ///< First list item log entry belongs to, if applicable.
  uintptr_t li2;  ///< Second list item log entry belongs to, if applicable.
  int len;  ///< List length when log entry was created.
  const char *action;  ///< Logged action.
} ListLogEntry;

typedef struct list_log ListLog;

/// List actions log
struct list_log {
  ListLog *next;  ///< Next chunk or NULL.
  size_t capacity;  ///< Number of entries in current chunk.
  size_t size;  ///< Current chunk size.
  ListLogEntry entries[];  ///< Actual log entries.
};
#endif
