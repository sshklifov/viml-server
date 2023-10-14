#pragma once

/// Special flags for base_arg
#define BASE_NONE 0          ///< Not a method (no base argument).
#define MAX_FUNC_ARGS 20     /// Maximum number of function arguments

/// Structure holding VimL function definition
typedef struct {
  const char* name;         ///< Name of the function.
  unsigned char min_argc;   ///< Minimal number of arguments.
  unsigned char max_argc;   ///< Maximal number of arguments.
  unsigned char base_arg;   ///< Method base arg # (1-indexed), BASE_NONE or BASE_LAST.
  bool fast;                ///< Can be run in |api-fast| events
} EvalFuncDef;

int find_internal_func_hash(const char *str, int len);

extern EvalFuncDef functions[];
