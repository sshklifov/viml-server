// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// eval.c: Expression evaluation.

#include "Eval.hpp"
#include "SkipFuncs.hpp"
#include "Options.hpp"

#include <cassert>
#include <cctype>
#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

const char* skip_expr(const char* arg) {
    EvalFactory dummy;
    dummy.setEvaluate(EvalFactory::EVAL_NONE);
    eval1(arg, dummy);
    return arg;
}

EvalExpr* eval1(const char*& arg, EvalFactory& factory) {
    // Get the first variable.
    EvalExpr* expr = eval2(arg, factory);

    if (arg[0] == '?') {
        // Get the second variable.
        arg = skipwhite(arg + 1);
        EvalExpr* bodyTrue = eval1(arg, factory);
        // Check for the ":".
        if (arg[0] != ':') {
            throw msg(arg, "Missing ':' after '?'");
        }
        // Get the third variable.
        arg = skipwhite(arg + 1);
        EvalExpr* bodyFalse = eval1(arg, factory);
        expr = factory.create<TernaryExpr>(expr, bodyTrue, bodyFalse);
    }

    return expr;
}

EvalExpr* eval2(const char*& arg, EvalFactory& factory) {
    // Get the first variable.
    EvalExpr* expr = eval3(arg, factory);

    // Repeat until there is no following "||".
    while (arg[0] == '|' && arg[1] == '|') {
        // Get the second variable.
        arg = skipwhite(arg + 2);
        EvalExpr* rhs = eval3(arg, factory);
        expr = factory.create<LogicOpExpr>(expr, rhs, '|');
    }
    return expr;
}

EvalExpr* eval3(const char*& arg, EvalFactory& factory) {
    // Get the first variable.
    EvalExpr* expr = eval4(arg, factory);

    // Repeat until there is no following "&&".
    while (arg[0] == '&' && arg[1] == '&') {
        // Get the second variable.
        arg = skipwhite(arg + 2);
        EvalExpr* rhs = eval4(arg, factory);
        expr = factory.create<LogicOpExpr>(expr, rhs, '&');
    }

    return expr;
}

EvalExpr* eval4(const char*& arg, EvalFactory& factory) {
    // Get the first variable.
    EvalExpr* expr = eval5(arg, factory);

    exprtype_T type = EXPR_UNKNOWN;
    int len = 2;
    switch (arg[0]) {
    case '=':
        if (arg[1] == '=') {
            type = EXPR_EQUAL;
        } else if (arg[1] == '~') {
            type = EXPR_MATCH;
        }
        break;
    case '!':
        if (arg[1] == '=') {
            type = EXPR_NEQUAL;
        } else if (arg[1] == '~') {
            type = EXPR_NOMATCH;
        }
        break;
    case '>':
        if (arg[1] != '=') {
            type = EXPR_GREATER;
            len = 1;
        } else {
            type = EXPR_GEQUAL;
        }
        break;
    case '<':
        if (arg[1] != '=') {
            type = EXPR_SMALLER;
            len = 1;
        } else {
            type = EXPR_SEQUAL;
        }
        break;
    case 'i':
        if (arg[1] == 's') {
            if (arg[2] == 'n' && arg[3] == 'o' && arg[4] == 't') {
                len = 5;
            }
            if (!vim_isIDc(arg[len])) {
                type = (len == 2 ? EXPR_IS : EXPR_ISNOT);
            }
        }
        break;
    }

    // If there is a comparative operator, use it.
    if (type != EXPR_UNKNOWN) {
        exprcase_T mod = CASE_OPTION;
        // extra '?' appended: ignore case
        // extra '#' appended: match case
        if (arg[len] == '?') {
            mod = CASE_IGNORE;
            len++;
        } else if (arg[len] == '#') {
            mod = CASE_MATCH;
            len++;
        }

        // Get the second variable.
        arg = skipwhite(arg + len);
        EvalExpr* rhs = eval5(arg, factory);
        expr = factory.create<CmpOpExpr>(expr, rhs, type, mod);
    }

    return expr;
}

EvalExpr* eval5(const char*& arg, EvalFactory& factory) {
    // Get the first variable.
    EvalExpr* expr = eval6(arg, factory);

    // Repeat computing, until no '+', '-' or '.' is following.
    for (;;) {
        int op = *arg;
        if (op != '+' && op != '-' && op != '.') {
            break;
        }

        // Get the second variable.
        if (op == '.' && arg[1] == '.') {  // ..string concatenation
            arg++;
        }
        arg = skipwhite(arg + 1);
        EvalExpr* rhs = eval6(arg, factory);
        expr = factory.create<BinOpExpr>(expr, rhs, op);
    }
    return expr;
}

EvalExpr* eval6(const char*& arg, EvalFactory& factory) {
    // Get the first variable.
    EvalExpr* expr = eval7(arg, factory);

    // Repeat computing, until no '*', '/' or '%' is following.
    for (;;) {
        int op = *arg;
        if (op != '*' && op != '/' && op != '%') {
            break;
        }

        // Get the second variable.
        arg = skipwhite(arg + 1);
        EvalExpr* rhs = eval7(arg, factory);
        expr = factory.create<BinOpExpr>(expr, rhs, op);
    }

    return expr;
}

EvalExpr* eval7(const char*& arg, EvalFactory& factory) {
    const char* start_leader = arg;
    while (*arg == '!' || *arg == '-' || *arg == '+') {
        arg = skipwhite(arg + 1);
    }
    const char *end_leader = arg;

    EvalExpr* expr = eval8(arg, factory);
    while (end_leader > start_leader) {
        end_leader--;
        // Skip unary +
        if (*end_leader == '!' || *end_leader == '-') {
            expr = factory.create<PrefixOpExpr>(expr, *end_leader);
        }
    }
    return expr;
}

EvalExpr* eval8(const char*& arg, EvalFactory& factory) {
    EvalExpr* expr = eval9(arg, factory);
    for (;;) {
        if (*arg == '-' && arg[1] == '>') {
            arg += 2;
            EvalExpr* name;
            if (*arg == '{') {
                // expr->{lambda}()
                name = get_lambda(arg, 0, factory);
            } else {
                // expr->name()
                if (strncmp(arg, "v:lua.", 6) == 0) {
                    int len = get_luafunc_name_len(arg + 6);
                    if (len > 0) {
                        const char* p = arg + 6 + len;
                        name = factory.create<SymbolExpr>(arg, p);
                        arg = p;
                    } else {
                        throw msg(arg, "Missing name after ->");
                    }
                } else {
                    name = get_id(arg, factory);
                }
            }
            // Common code for both methods
            if (*arg != '(') {
                if (*skipwhite(arg) == '(') {
                    throw msg(arg, "No white space allowed before parenthesis");
                } else {
                    throw msg(arg, "Missing parentheses");
                }
            }
            expr = get_func(arg, name, expr, factory);
        } else if (*arg == '(') {
            expr = get_func(arg, expr, nullptr, factory);
        } else if (*arg == '[') {
            expr = get_index(arg, expr, factory);
        } else if (*arg == '.') {
            const char* p = arg + 1;
            while (eval_isdictc(*p)) {
                ++p;
            }
            if (p - arg <= 1) {
                break;
            } else {
                EvalExpr* idx = factory.create<LiteralExpr>(VAR_STRING, arg + 1, p);
                expr = factory.create<IndexExpr>(expr, idx);
                arg = p;
            }
        } else {
            break;
        }
    }

    arg = skipwhite(arg);
    return expr;
}

EvalExpr* eval9(const char*& arg, EvalFactory& factory) {
    EvalExpr* expr;
    int not_done = false;
    switch (*arg) {
    // Number constant.
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
        VarType type = VAR_UNKNOWN;
        const char* p = skip_numerical(arg, type);
        expr = factory.create<LiteralExpr>(type, arg, p);
        arg = p;
        break;
    }

    // String constant: "string".
    case '"': {
        const char* p = skip_string(arg);
        expr = factory.create<LiteralExpr>(VAR_STRING, arg, p);
        arg = p;
        break;
    }

    // Literal string constant: 'str''ing'.
    case '\'': {
        const char* p = skip_lit_string(arg);
        expr = factory.create<LiteralExpr>(VAR_STRING, arg, p);
        arg = p;
        break;
    }

    // List: [expr, expr]
    case '[':
        expr = get_list(arg, factory);
        break;

    // Dictionary: #{key: val, key: val}
    case '#':
        if (arg[1] == '{') {
            arg++;
            expr = get_dict_or_expand(arg, true, factory);
        } else {
            not_done = true;
        }
        break;

    // Lambda: {arg, arg -> expr}
    // Dictionary: {'key': val, 'key': val}
    case '{':
        expr = get_lambda(arg, 1, factory);
        break;

    // Option value: &name
    case '&':
    // Environment variable: $VAR.
    case '$':
    // Register contents: @r.
    case '@':
        expr = get_var_special(arg, factory);
        break;

    // nested expression: (expression).
    case '(': {
        arg = skipwhite(arg + 1);
        expr = eval1(arg, factory);
        if (*arg != ')') {
            throw msg(arg, "Missing ')'");
        }
        arg++;
        break;
    }

    default:
        not_done = true;
        break;
    }

    if (not_done) {
        // Must be a variable or function name.
        // Can also be a curly-braces kind of name: {expr}.
        if (eval_isnamec1(*arg) || *arg == '{' || get_fname_script_len(arg) > 0) {
            expr = get_id(arg, factory);
        } else {
            throw msg(arg, "Invalid expression");
        }
    }

    return expr;
}

EvalExpr* get_list(const char*& arg, EvalFactory& factory) {
    Vector<EvalExpr*> exprs;

    arg = skipwhite(arg + 1);
    while (*arg != ']' && *arg != NUL) {
        EvalExpr* expr = eval1(arg, factory);
        exprs.emplace(expr);

        if (*arg == ']') {
            break;
        }
        if (*arg != ',') {
            throw msg(arg, "Missing comma in List");
        }
        arg = skipwhite(arg + 1);
    }

    if (*arg != ']') {
        throw msg(arg, "Missing end of list");
    }

    arg++;
    return factory.create<ListExpr>(std::move(exprs));
}

Vector<SymbolExpr*> get_function_args(const char*& arg, const bool lambda, EvalFactory& factory) {
    const char endchar = (lambda ? '-' : ')');

    Vector<SymbolExpr*> res;
    bool any_default = false;
    bool mustend = false;
    bool maybe_dict = lambda;

    // Isolate the arguments: "arg1, arg2, ...)"
    while (*arg != endchar) {
        const char* varname = arg;
        int len = 0;
        if (arg[0] == '.' && arg[1] == '.' && arg[2] == '.') {
            if (lambda) {
                throw msg(arg, "... not allowed");
            }
            maybe_dict = false;
            mustend = true;

            len = 3;
            arg += len;
        } else {
            while (ASCII_ISALNUM(arg[len]) || arg[len] == '_') {
                len++;
            }
            if (len == 0 || isdigit(*arg)
                || (len == 9 && strncmp("firstline", arg, len) == 0)
                || (len == 8 && strncmp("lastline", arg, len) == 0)) {
                if (maybe_dict) {
                    return {};
                } else {
                    throw msg(arg, "Illegal argument");
                }
            }

            StringView varView(varname, len);
            for (int i = 0; i < res.count(); ++i) {
                SymbolExpr* other = res[i];
                if (other->name == varView) {
                    throw msg(arg, "Duplicate argument name");
                }
            }
            arg += len;

            // Skip default args
            const char* p = skipwhite(arg);
            if (*p == '=') {
                maybe_dict = false;
                any_default = true;
                if (lambda) {
                    throw msg(p, "No default arguments allowed");
                }
                arg = skipwhite(p + 1);
                arg = skip_expr(arg); //  TODO don't skip
                // Trim trailing whitespace, once is enough
                if (ascii_iswhite(arg[-1])) {
                    arg--;
                }
            } else if (any_default && !maybe_dict) {
                throw msg(arg, "Non-default argument follows default argument");
            }

            if (*arg == ',') {
                arg++;
                maybe_dict = false;
            } else {
                if (ascii_iswhite(*arg) && *skipwhite(arg) == ',') {
                    throw msg(arg, "No white space allowed before ','");
                } else {
                    mustend = true;
                }
            }
        }
        arg = skipwhite(arg);
        if (mustend && *arg != endchar) {
            if (!maybe_dict) {
                if (endchar == '-') {
                    throw msg(arg, "Expecting '->'");
                } else {
                    throw msg(arg, "Expecting ')'");
                }
            } else {
                return {}; // Not a function
            }
        }
        // Now, finally add the variable
        res.emplace(factory.create<SymbolExpr>(varname, varname + len));
    }

    return res;
}

EvalExpr* get_lambda(const char*& arg, bool maybe_dict, EvalFactory& factory) {
    // First, check if this is a lambda expression. "->" must exists.
    const char* orig_arg = arg;
    arg = skipwhite(arg + 1);
    Vector<SymbolExpr*> fargs = get_function_args(arg, true, factory);
    if (*arg != '-' || arg[1] != '>') {
        if (maybe_dict) {
            arg = orig_arg;
            return get_dict_or_expand(arg, 0, factory);
        } else {
            throw msg(arg, "Expecting ->");
        }
    }

    // Get the start and the end of the expression.
    arg = skipwhite(arg + 2);
    EvalExpr* body = eval1(arg, factory);
    if (*arg != '}') {
        throw msg(arg, "Expecting '}'");
    }
    ++arg;
    return factory.create<LambdaExpr>(std::move(fargs), body);
}

EvalExpr* get_dict_or_expand(const char*& arg, bool literal, EvalFactory& factory) {
    assert(*arg == '{');
    Vector<DictExpr::Pair> dict;

    // First check if it's not a curly-braces expression: {expr}.
    // This mens we need to call eval1 and branch the result into
    // either an expansion or the first dictionary item.
    // "{}" is an empty Dictionary.
    // "#{abc}" is never a curly-braces expression.
    arg = skipwhite(arg + 1);
    if (*arg != '}' && !literal) {
        EvalExpr* expr = eval1(arg, factory);
        if (*arg == '}') {
            throw msg(arg, "TODO: Hard case to solve");
#if 0
            ++arg;
            return accum_expanded(arg, start, expr, factory);
#endif
        } else {
            if (*arg != ':') {
                throw msg(arg, "Missing colon in Dictionary");
            }
            arg = skipwhite(arg + 1);
            EvalExpr* val = eval1(arg, factory);
            dict.emplace(expr, val);
            if (*arg != '}') {
                if (*arg != ',') {
                    throw msg(arg, "Missing comma in Dictionary");
                }
                arg = skipwhite(arg + 1);
            }
        }
    }

    while (*arg != '}' && *arg != NUL) {
        EvalExpr* key;
        if (literal) {
            int len = get_literal_key_len(arg);
            if (len > 0) {
                key = factory.create<LiteralExpr>(VAR_STRING, arg, len);
                arg = skipwhite(arg + len);
            } else {
                throw msg(arg, "Expecting Dictionary key");
            }
        } else {
            key = eval1(arg, factory);
        }

        if (*arg != ':') {
            throw msg(arg, "Missing colon in Dictionary");
        }
        arg = skipwhite(arg + 1);

        EvalExpr* val = eval1(arg, factory);
        dict.emplace(key, val);

        if (*arg == '}') {
            break;
        }
        if (*arg != ',') {
            throw msg(arg, "Missing comma in Dictionary");
        }
        arg = skipwhite(arg + 1);
    }

    if (*arg != '}') {
        throw msg(arg, "Missing end of Dictionary '}'");
    }

    arg++;
    return factory.create<DictExpr>(std::move(dict));
}

EvalExpr* get_id(const char*& arg, EvalFactory& factory) {
    const char* start = arg;
    if (*start == AUTOLOAD_CHAR) {
        throw msg(start, "Expecting path");
    } else if (*start == ':') {
        throw msg(start, "Expecting scope");
    } else if (ascii_isdigit(*start)) {
        throw msg(start, "Bad name");
    }

    // No curly expansions
    int has_scope = 0;
    int len = get_id_part_len(start, arg, has_scope);
    if (len > 0 && start[len] != '{') {
        arg += len;
        EvalExpr* res = factory.create<SymbolExpr>(start, arg);
        return res;
    }
    arg += len;

    FStr pattern(start, arg);
    EvalFactory depNames(EvalFactory::EVAL_SYMBOLS_ONLY);
    while (eval_isnamec(*arg) || *arg == '{') {
        if (*arg == '{') {
            pattern.append("{}");
            arg++; //< Skip the '{'
            eval1(arg, depNames);
            if (*arg != '}') {
                throw msg(arg, "Expecting end of curly braces name '}'");
            }
            arg++; //< Skip the '}'
        } else {
            int len = get_id_part_len(start, arg, has_scope);
            pattern.append(arg, len);
            arg += len;
        }
    }
    // TODO
    return factory.create<SymbolExpr>(start, arg, std::move(pattern), std::move(depNames));
}

EvalExpr* get_func(const char*& arg, EvalExpr* name, EvalExpr* base, EvalFactory& factory) {
    if (name) {
        SymbolExpr* sym = name->cast<SymbolExpr>();
        if (sym) {
            sym->setFlags(SymbolExpr::FUNCTION);
        }
    }

    Vector<EvalExpr*> fargs;
    // Get the remaining arguments.
    do {
        arg = skipwhite(arg + 1); // skip the '(' or ','
        if (*arg == NUL || *arg == ',' || *arg == ')') {
            break;
        }
        EvalExpr* expr = eval1(arg, factory);
        fargs.emplace(expr);
    } while (*arg == ',');

    if (*arg == ',') {
        throw msg(arg, "Unexpected ','");
    } else if (*arg != ')') {
        throw msg(arg, "Expecting argument");
    }
    arg++;

    if (base) {
        return factory.create<MethodExpr>(name, base, std::move(fargs));
    } else {
        return factory.create<InvokeExpr>(name, std::move(fargs));
    }
}

EvalExpr* get_index(const char*& arg, EvalExpr* var, EvalFactory& factory) {
    // Get the (first) variable from inside the [].
    EvalExpr *idx1 = nullptr, *idx2 = nullptr;
    bool range = false;
    arg = skipwhite(arg + 1);
    if (*arg != ':') {
        idx1 = eval1(arg, factory);
    }

    // Get the second variable from inside the [:].
    if (*arg == ':') {
        range = true;
        arg = skipwhite(arg + 1);
        if (*arg != ']') {
            idx2 = eval1(arg, factory);
        }
    }

    // Check for the ']'.
    if (*arg != ']') {
        throw msg(arg, "Missing ']'");
    }
    arg++;

    if (range) {
        return factory.create<IndexRangeExpr>(var, idx1, idx2);
    } else {
        return factory.create<IndexExpr>(var, idx1);
    }
}

EvalExpr* get_var_indexed(const char*& p, EvalFactory& f) {
    EvalExpr* res = get_id(p, f);
    while (*p == '[' || *p == '.') {
        if (*p == '[') {
            res = get_index(p, res, f);
        } else {
            int len = get_dict_key_len(p + 1);
            if (len <= 0) {
                break;
            }
            p++;
            EvalExpr* idx = f.create<LiteralExpr>(VAR_STRING, p, len);
            res = f.create<IndexExpr>(res, idx);
            p += len;
        }
    }
    return res;
}

SymbolExpr* get_var_special(const char*& arg, EvalFactory& factory) {
    SymbolExpr* expr;
    if (*arg == '&') {
        const char* p = skip_option_scope(arg);
        int len = get_option_len(p);
        VarType type = get_option_type(p, len);
        if (type == VAR_UNKNOWN) {
            throw msg(arg, "Unknown option");
        }
        expr = factory.create<SymbolExpr>(arg, p + len);
        arg = p + len;
    } else if (*arg == '$') {
        const char* p = arg + 1; // Skip $
        int len = get_env_len(p);
        if (len == 0) {
            throw msg(arg, "Expecting environment name");
        }
        expr = factory.create<SymbolExpr>(arg, p + len);
        arg = p + len;
    } else {
        int len = valid_yank_reg(arg[1]);
        if (eval_isnamec1(arg[1]) && eval_isnamec(arg[2])) {
            throw msg(arg, "Invalid register name");
        }
        len++; // include the "@"
        expr = factory.create<SymbolExpr>(arg, arg + len);
        arg += len;
    }
    return expr;
}

EvalExpr* get_var_list_one(const char*& arg, bool allow_special, EvalFactory& factory) {
    if (*arg == '@' || *arg == '$' || *arg == '&') {
        if (!allow_special) {
            throw msg(arg, "Options, settings and env variables not allowed");
        }
        return get_var_special(arg, factory);
    } else {
        return get_var_indexed(arg, factory);
    }
}

Vector<EvalExpr*> get_var_list(const char*& arg, bool allow_special, int& semicolon, EvalFactory& factory) {
    Vector<EvalExpr*> vars;

    semicolon = 0;
    if (*arg == '[') {
        // "[var, var]": find the matching ']'.
        for (;;) {
            arg = skipwhite(arg + 1); // skip whites after '[', ';' or ','
            vars.emplace(get_var_list_one(arg, allow_special, factory));
            arg = skipwhite(arg);
            if (*arg == ']') {
                break;
            } else if (*arg == ';') {
                if (semicolon == 1) {
                    throw msg(arg, "Double ; in list of variables");
                }
                semicolon = 1;
            } else if (*arg != ',') {
                throw msg(arg, "Invalid argument");
            }
        }
        arg++;
    } else {
        vars.emplace(get_var_list_one(arg, allow_special, factory));
    }
    return vars;
}
