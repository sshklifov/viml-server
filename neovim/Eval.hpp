#pragma once

#include "Ascii.hpp"

#include <EvalExprDefs.hpp>
#include <DiagnosticReporter.hpp>

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

struct EvalFactory {
    EvalFactory() : evaluate(1) {}
    EvalFactory(const EvalFactory&) = delete;
    EvalFactory(EvalFactory&&) = delete;

    ~EvalFactory() {
        clear();
    }

    void clear() {
        for (EvalExpr* expr : exprs) {
            delete expr;
        }
        exprs.clear();
    }

    void setEvaluate(int state) {
        evaluate = state;
    }

    template <typename T, typename ... Args>
    T* create(Args&&... args) {
        if (!evaluate) {
            T* res = new T(std::forward<Args>(args)...);
            exprs.emplace(res);
            return res;
        } else {
            return nullptr;
        }
    }

private:
    Vector<EvalExpr*> exprs;
    int evaluate;
};

/// Skip over an expression at "arg".
///
/// @return  pointer to after expression.
const char* skip_expr(const char* arg);

/// Handle top level expression:
///      expr2 ? expr1 : expr1
///
/// "arg" must point to the first non-white of the expression.
/// "arg" is advanced to the next non-white after the recognized expression.
EvalExpr* eval1(const char*& arg, EvalFactory& factory);

/// Handle first level expression:
///      expr2 || expr2 || expr2     logical OR
///
/// "arg" must point to the first non-white of the expression.
/// "arg" is advanced to the next non-white after the recognized expression.
///
EvalExpr* eval2(const char*& arg, EvalFactory& factory);

/// Handle second level expression:
///      expr3 && expr3 && expr3     logical AND
///
/// @param arg  must point to the first non-white of the expression.
///             `arg` is advanced to the next non-white after the recognized expression.
EvalExpr* eval3(const char*& arg, EvalFactory& factory);


/// Handle third level expression:
///      var1 == var2
///      var1 =~ var2
///      var1 != var2
///      var1 !~ var2
///      var1 > var2
///      var1 >= var2
///      var1 < var2
///      var1 <= var2
///      var1 is var2
///      var1 isnot var2
///
/// "arg" must point to the first non-white of the expression.
/// "arg" is advanced to the next non-white after the recognized expression.
EvalExpr* eval4(const char*& arg, EvalFactory& factory);


/// Handle fourth level expression:
///      +       number addition, concatenation of list or blob
///      -       number subtraction
///      .       string concatenation
///      ..      string concatenation
///
/// @param arg  must point to the first non-white of the expression.
///             `arg` is advanced to the next non-white after the recognized expression.
EvalExpr* eval5(const char*& arg, EvalFactory& factory);

/// Handle fifth level expression:
///  - *  number multiplication
///  - /  number division
///  - %  number modulo
///
/// @param[in,out]  arg  Points to the first non-whitespace character of the
///                      expression.  Is advanced to the next non-whitespace
///                      character after the recognized expression.
EvalExpr* eval6(const char*& arg, EvalFactory& factory);

/// Handle sixth level expression:
///  ! in front  logical NOT
///  - in front  unary minus
///  + in front  unary plus (ignored)
///
/// "arg" must point to the first non-white of the expression.
/// "arg" is advanced to the next non-white after the recognized expression.
EvalExpr* eval7(const char*& arg, EvalFactory& factory);

/// Handle eight level expression:
///  trailing []  subscript in String or List
///  trailing .name entry in Dictionary
///  function()  function call
///  trailing ->name()  method call
EvalExpr* eval8(const char*& arg, EvalFactory& factory);

/// Handle ninth level expression:
///  number  number constant
///  0zFFFFFFFF  Blob constant
///  "string"  string constant
///  'string'  literal string constant
///  &option-name option value
///  @r   register contents
///  identifier  variable value
///  $VAR  environment variable
///  (expression) nested expression
///  [expr, expr] List
///  {key: val, key: val}  Dictionary
///  #{key: val, key: val}  Dictionary with literal keys
///
/// NB: Does not skip whitespaces! This is required for eval8.
EvalExpr* eval9(const char*& arg, EvalFactory& factory);

/// Allocate a variable for a List and fill it from "arg".
EvalExpr* get_list(const char*& arg, EvalFactory& factory);

/// Get function arguments.
/// Advances "arg" to "endchar" on success.
Vector<FStr> get_function_args(const char*& arg, char endchar, EvalFactory& factory);

/// Parse an expression starting with curly bracket
/// The parse flow is as follows:
///   get_curly:          Try to parse as lambda, if
///                       failed, call get_dict_or_expand.
///
///   get_dict_or_expand: Try to parse as dict, if failed,
///                       call accum_expanded
///
///   accum_expanded:     Call get_expanded_part and
///                       accumulate parts
EvalExpr* get_curly(const char*& arg, EvalFactory& factory);

/// Allocate a variable for a Dictionary and fill it from "arg".
/// "literal" is true for #{key: val}.
/// "arg" might point to a curly name instead, in which case it
/// will be expanded.
EvalExpr* get_dict_or_expand(const char*& arg, bool literal, EvalFactory& factory);

/// Generates one expansion of 'magic' {}'.
/// Must be called multiple times while to expand all 'magic's.
EvalExpr* get_expanded_part(const char*& arg, int allow_scope, EvalFactory& factory);

/// Expands out the 'magic' {}'s in a variable/function name.
/// Accumulates the expanded parts from get_expanded_part.
EvalExpr* accum_expanded(const char*& arg, EvalExpr* res, EvalFactory& factory);

/// Allocate an expression invoking the function.
///
/// @param arg   argument, pointing to the '('
/// @param name  name of the function
/// @param base  first function argument (optional, used for methods)
EvalExpr* get_func(const char*& arg, EvalExpr* name, EvalExpr* base, EvalFactory& factory);

/// Evaluate an "[expr]" or "[expr:expr]" index.    Also "dict.key".
/// "*arg" points to the '[' or '.'.
EvalExpr* get_index(const char*& arg, EvalExpr* what, EvalFactory& factory);
