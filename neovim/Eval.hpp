#pragma once

#include "EvalExprDefs.hpp"

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
///  function()  function call
///  trailing ->name()  method call
/// NB: NO trailing .name entry in Dictionary
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
Vector<SymbolExpr*> get_function_args(const char*& arg, const bool lambda, EvalFactory& factory);

/// Parse a lambda expression and get a Funcref from "arg".
EvalExpr* get_lambda(const char*& arg, bool force, EvalFactory& factory);

/// Allocate a variable for a Dictionary and fill it from "arg".
/// "literal" is true for #{key: val}.
/// "arg" might point to a curly name instead, in which case it
/// will be expanded.
EvalExpr* get_dict_or_expand(const char*& arg, bool literal, EvalFactory& factory);

/// Expands out the 'magic' {}'s in a variable/function name.
EvalExpr* get_id(const char*& arg, EvalFactory& factory);

/// Allocate an expression invoking the function.
///
/// @param arg   argument, pointing to the '('
/// @param name  name of the function
/// @param base  first function argument (optional, used for methods)
EvalExpr* get_func(const char*& arg, EvalExpr* name, EvalExpr* base, EvalFactory& factory);

/// Evaluate an "[expr]" or "[expr:expr]" index.
/// "*arg" points to the '['.
EvalExpr* get_index(const char*& arg, EvalExpr* var, EvalFactory& factory);

/// Parse one name with trailing indices
EvalExpr* get_var_indexed(const char*& arg, EvalFactory& factory);

/// Parse one (register, env or option) variable
SymbolExpr* get_var_special(const char*& arg, EvalFactory& factory);

/// Parse one variable
EvalExpr* get_var_list_one(const char*& arg, bool allow_special, EvalFactory& factory);

/// Skip over assignable variable "var" or list of variables "[var, var]".
/// Used for ":let varvar = expr" and ":for varvar in expr".
Vector<EvalExpr*> get_var_list(const char*& arg, bool allow_special, int& semicolon, EvalFactory& factory);
