// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// eval.c: Expression evaluation.

#include "Eval.hpp"
#include "Charset.hpp"
#include "Help.hpp" // TODO rename idk
#include "Options.hpp"

#include <cassert>
#include <cctype>
#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

EvalExpr* Evaluator::eval(const char* arg, int& numRead) {
    arg = skipwhite(arg);
    try {
        // TODO num read what used for
        EvalExpr* res = eval1(arg);
        return res;
    } catch (const char* err) {
        fprintf(stderr, "%s: \"%s\"\n", err, arg);
        return nullptr;
    }
}

EvalExpr* Evaluator::eval1(const char*& arg) {
    // Get the first variable.
    EvalExpr* expr = eval2(arg);

    if (arg[0] == '?') {
        bool result = false;
        // Get the second variable.
        arg = skipwhite(arg + 1);
        EvalExpr* bodyTrue = eval1(arg);
        // Check for the ":".
        if (arg[0] != ':') {
            throw "Missing ':' after '?'";
        }
        // Get the third variable.
        arg = skipwhite(arg + 1);
        EvalExpr* bodyFalse = eval1(arg);
        expr = factory->create<TernaryNode>(expr, bodyTrue, bodyFalse);
    }

    return expr;
}

EvalExpr* Evaluator::eval2(const char*& arg) {
    // Get the first variable.
    EvalExpr* expr = eval3(arg);

    // Repeat until there is no following "||".
    while (arg[0] == '|' && arg[1] == '|') {
        // Get the second variable.
        arg = skipwhite(arg + 2);
        EvalExpr* rhs = eval3(arg);
        expr = factory->create<LogicOpNode>(expr, rhs, LogicOpNode::OR);
    }
    return expr;
}

EvalExpr* Evaluator::eval3(const char*& arg) {
    // Get the first variable.
    EvalExpr* expr = eval4(arg);

    // Repeat until there is no following "&&".
    while (arg[0] == '&' && arg[1] == '&') {
        // Get the second variable.
        arg = skipwhite(arg + 2);
        EvalExpr* rhs = eval4(arg);
        expr = factory->create<LogicOpNode>(expr, rhs, LogicOpNode::AND);
    }

    return expr;
}

EvalExpr* Evaluator::eval4(const char*& arg) {
    // Get the first variable.
    EvalExpr* expr = eval5(arg);

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
        int mod = 0;
        // extra '?' appended: ignore case
        // extra '#' appended: match case
        if (arg[len] == '?' || arg[len] == '#') {
            mod = arg[len];
            len++;
        }

        // Get the second variable.
        arg = skipwhite(arg + len);
        EvalExpr* rhs = eval5(arg);
        expr = factory->create<CmpOpNode>(expr, rhs, type, mod);
    }

    return expr;
}

EvalExpr* Evaluator::eval5(const char*& arg) {
    // Get the first variable.
    EvalExpr* expr = eval6(arg);

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
        EvalExpr* rhs = eval6(arg);
        expr = factory->create<ArithOpNode>(expr, rhs, op);
    }
    return expr;
}

EvalExpr* Evaluator::eval6(const char*& arg) {
    // Get the first variable.
    EvalExpr* expr = eval7(arg);

    // Repeat computing, until no '*', '/' or '%' is following.
    for (;;) {
        int op = *arg;
        if (op != '*' && op != '/' && op != '%') {
            break;
        }

        // Get the second variable.
        arg = skipwhite(arg + 1);
        EvalExpr* rhs = eval7(arg);
        expr = factory->create<ArithOpNode>(expr, rhs, op);
    }

    return expr;
}

EvalExpr* Evaluator::eval7(const char*& arg) {
    const char* start_leader = arg;
    while (*arg == '!' || *arg == '-' || *arg == '+') {
        arg = skipwhite(arg + 1);
    }
    const char *end_leader = arg;

    EvalExpr* expr = eval8(arg);
    while (end_leader > start_leader) {
        end_leader--;
        // Skip unary +
        if (*end_leader == '!' || *end_leader == '-') {
            expr = factory->create<PrefixOpNode>(expr, *end_leader);
        }
    }
    return expr;
}

EvalExpr* Evaluator::eval8(const char*& arg) {
    EvalExpr* expr = eval9(arg);
    // "." is ".name" lookup when we found a dict.
    for (;;) {
        if (*arg == '-' && arg[1] == '>') {
            arg += 2;
            EvalExpr* name;
            if (*arg == '{') {
                // expr->{lambda}()
                name = get_curly(arg);
            } else {
                // expr->name()
                if (strncmp(arg, "v:lua.", 6) == 0) {
                    const char* lua_funcname = arg + 6;
                    arg = skip_luafunc_name(lua_funcname);
                    if (arg > lua_funcname) {
                        FStr s(lua_funcname, arg);
                        name = factory->create<BaseNode>(VAR_STRING, std::move(s));
                        name = factory->create<NameNode>(expr);
                    } else {
                        throw "Missing name after ->";
                    }
                } else {
                    name = get_expanded_part(arg, 1);
                    name = accum_expanded(arg, name);
                }
            }
            // Common code for both methods
            if (*arg != '(') {
                if (*skipwhite(arg) == '(') {
                    throw "No white space allowed before parenthesis";
                } else {
                    throw "Missing parentheses";
                }
            }
            expr = get_func(arg, name, expr);
        } else if (*arg == '(') {
            expr = get_func(arg, expr);
        } else if (*arg == '[' || *arg == '.') {
            // TODO assume it is always dict
            expr = get_index(arg, expr);
        } else {
            break;
        }
    }

    arg = skipwhite(arg);
    return expr;
}

EvalExpr* Evaluator::eval9(const char*& arg) {
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
        int type = VAR_UNKNOWN;
        const char* p = skip_numerical(arg, &type);
        expr = factory->create<BaseNode>(type, FStr(arg, p));
        arg = p;
        break;
    }

    // String constant: "string".
    case '"': {
        const char* p = skip_string(arg);
        expr = factory->create<BaseNode>(VAR_STRING, FStr(arg, p));
        arg = p;
        break;
    }

    // Literal string constant: 'str''ing'.
    case '\'': {
        const char* p = skip_lit_string(arg);
        expr = factory->create<BaseNode>(VAR_STRING, FStr(arg, p));
        arg = p;
        break;
    }

    // List: [expr, expr]
    case '[':
        expr = get_list(arg);
        break;

    // Dictionary: #{key: val, key: val}
    case '#':
        if (arg[1] == '{') {
            arg++;
            expr = get_dict_or_expand(arg, true);
        } else {
            not_done = true;
        }
        break;

    // Lambda: {arg, arg -> expr}
    // Dictionary: {'key': val, 'key': val}
    case '{':
        expr = get_curly(arg);
        break;

    // Option value: &name
    case '&': {
        const char* p = skip_option_scope(arg);
        int len = get_option_len(p);
        int type = OptionsMap::getSingleton().findVarType(p, len);
        if (type == VAR_UNKNOWN) {
            throw "Unknown option";
        }
        expr = factory->create<BaseNode>(type, FStr(arg, p + len));
        arg = p + len;
        break;
    }

    // Environment variable: $VAR.
    case '$': {
        const char* p = arg + 1; // Skip $
        int len = get_env_len(p);
        if (len == 0) {
            throw "Expecting environment name";
        }
        expr = factory->create<BaseNode>(VAR_STRING, FStr(arg, p + len));
        arg = p + len;
        break;
    }

    // Register contents: @r.
    case '@': {
        int len = 1 + valid_yank_reg(arg[1]);
        expr = factory->create<BaseNode>(VAR_STRING, FStr(arg, len));
        arg += len;
        break;
    }

    // nested expression: (expression).
    case '(': {
        arg = skipwhite(arg + 1);
        EvalExpr* expr = eval1(arg);
        if (*arg != ')') {
            throw "Missing ')'";
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
        expr = get_expanded_part(arg, 1);
        expr = accum_expanded(arg, expr);
    }

    return expr;
}

EvalExpr* Evaluator::get_list(const char*& arg) {
    Vector<EvalExpr*> exprs;

    arg = skipwhite(arg + 1);
    while (*arg != ']' && *arg != NUL) {
        EvalExpr* expr = eval1(arg);
        exprs.emplace(expr);

        if (*arg == ']') {
            break;
        }
        if (*arg != ',') {
            throw "Missing comma in List";
        }
        arg = skipwhite(arg + 1);
    }

    if (*arg != ']') {
        throw "Missing end of list";
    }

    arg++;
    return factory->create<ListNode>(std::move(exprs));
}

Vector<FStr> Evaluator::get_function_args(const char*& arg, char endchar) {
    // TODO these two
    int default_args = false;
    int varargs = false;

    Vector<FStr> res;
    bool any_default = false;
    bool any_args = false;
    bool mustend = false;

    // Isolate the arguments: "arg1, arg2, ...)"
    while (*arg != endchar) {
        if (arg[0] == '.' && arg[1] == '.' && arg[2] == '.') {
            varargs = true;
            any_args = true;
            mustend = true;
            arg += 3;
        } else {
            int len = 0;
            while (ASCII_ISALNUM(arg[len]) || arg[len] == '_') {
                len++;
            }
            FStr a(arg, len);
            if (a.empty() || isdigit(*a.str()) || a == "firstline" || a == "lastline") {
                if (any_args) {
                    throw "Illegal argument";
                } else {
                    return {}; // Not a function
                }
            }
            for (int i = 0; i < res.count(); ++i) {
                if (a == res[i]) {
                    throw "Duplicate argument name";
                }
            }
            res.emplace(std::move(a));
            arg += len;

            // Skip default args
            if (default_args && *skipwhite(arg) == '=') {
                any_default = true;
                arg = skipwhite(arg) + 1;
                arg = skipwhite(arg);
                EvalExpr* ret = eval1(arg);
                // Trim trailing whitespace
                while (ascii_iswhite(arg[-1])) {
                    arg--;
                }
            } else if (any_default) {
                throw "Non-default argument follows default argument";
            }

            if (*arg == ',') {
                arg++;
                any_args = true;
            } else {
                if (ascii_iswhite(*arg) && *skipwhite(arg) == ',') {
                    throw "No white space allowed before ','";
                } else {
                    mustend = true;
                }
            }
        }
        arg = skipwhite(arg);
        if (mustend && *arg != endchar) {
            if (any_args) {
                bool is_lambda = (endchar == '-');
                throw (is_lambda ? "Expecting '->'" : "Expecting '('");
            } else {
                return {}; // Not a function
            }
        }
    }

    return res;
}

EvalExpr* Evaluator::get_curly(const char*& arg) {
    // First, check if this is a lambda expression. "->" must exists.
    const char* orig_arg = arg;
    arg = skipwhite(arg + 1);
    Vector<FStr> fargs = get_function_args(arg, '-');
    if (*arg != '-' || arg[1] != '>') {
        arg = orig_arg;
        return get_dict_or_expand(arg, 0);
    }

    // Get the start and the end of the expression.
    arg = skipwhite(arg + 2);
    EvalExpr* body = eval1(arg);
    if (*arg != '}') {
        throw "Expecting '}'";
    }
    ++arg;
    return factory->create<LambdaNode>(std::move(fargs), body);
}

EvalExpr* Evaluator::get_dict_or_expand(const char*& arg, bool literal) {
    assert(*arg == '{');
    Vector<DictNode::Pair> dict;

    // First check if it's not a curly-braces expression: {expr}.
    // This mens we need to call eval1 and branch the result into
    // either an expansion or the first dictionary item.
    // "{}" is an empty Dictionary.
    // "#{abc}" is never a curly-braces expression.
    arg = skipwhite(arg + 1);
    if (*arg != '}' && !literal) {
        EvalExpr* expr = eval1(arg);
        if (*arg == '}') {
            return accum_expanded(arg, expr);
        } else {
            if (*arg != ':') {
                throw "Missing colon in Dictionary";
            }
            arg = skipwhite(arg + 1);
            EvalExpr* val = eval1(arg);
            dict.emplace(expr, val);
            if (*arg != '}') {
                if (*arg != ',') {
                    throw "Missing comma in Dictionary";
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
                key = factory->create<BaseNode>(VAR_STRING, FStr(arg, len));
                arg = skipwhite(arg + len);
            } else {
                throw "Expecting Dictionary key";
            }
        } else {
            key = eval1(arg);
        }

        if (*arg != ':') {
            throw "Missing colon in Dictionary";
        }
        arg = skipwhite(arg + 1);

        EvalExpr* val = eval1(arg);
        dict.emplace(key, val);

        if (*arg == '}') {
            break;
        }
        if (*arg != ',') {
            throw "Missing comma in Dictionary";
        }
        arg = skipwhite(arg + 1);
    }

    if (*arg != '}') {
        throw "Missing end of Dictionary '}'";
    }

    arg++;
    return factory->create<DictNode>(std::move(dict));
}

EvalExpr* Evaluator::get_expanded_part(const char*& arg, int allow_scope) {
    int len = get_id_len(arg, allow_scope);
    if (len > 0) {
        EvalExpr* res = factory->create<BaseNode>(VAR_STRING, FStr(arg, len));
        arg += len;
        return res;
    }
    if (*arg == '{') {
        EvalExpr* expr = eval1(arg);
        if (*arg != '}') {
            throw "Expecting end of curly braces name '}'";
        }
        ++arg;
        return expr;
    }
    throw "Expecting variable or function";
}

EvalExpr* Evaluator::accum_expanded(const char*& arg, EvalExpr* res) {
    if (eval_isnamec1(*arg) || *arg == '#' || *arg == '{') {
        EvalExpr* part = get_expanded_part(arg, 0);
        res = factory->create<ArithOpNode>(res, part, '.');
        return accum_expanded(arg, res);
    } else {
        return factory->create<NameNode>(res);
    }
}

EvalExpr* Evaluator::get_func(const char*& arg, EvalExpr* name, EvalExpr* base) {
    Vector<EvalExpr*> fargs;
    if (base) {
        fargs.emplace(base);
    }

    // Get the remaining arguments.
    do {
        arg = skipwhite(arg + 1); // skip the '(' or ','
        if (*arg == ')' || *arg == ',' || *arg == NUL) {
            break;
        }
        EvalExpr* expr = eval1(arg);
        if (!expr) {
            return 0; // < fail
        }
        fargs.emplace(expr);
        if (*arg != ',') {
            break;
        }
    } while (*arg == ',');

    if (*arg == ')') {
        arg++;
    } else {
        return 0; // < fail
    }

    return factory->create<InvokeNode>(name, std::move(fargs));
}

EvalExpr* Evaluator::get_index(const char*& arg, EvalExpr* what) {
    if (*arg == '.') {
        // dict.name
        arg++;
        const char* p = arg;
        while (eval_isdictc(*p)) {
            ++p;
        }
        if (p == arg) {
            throw "Expecting Dictionary key";
        } else {
            FStr key(arg, p);
            EvalExpr* idx = factory->create<BaseNode>(VAR_STRING, std::move(key));
            return factory->create<IndexNode>(what, idx);
        }
    } else {
        // something[idx]
        //
        // Get the (first) variable from inside the [].
        EvalExpr *idx1 = nullptr, *idx2 = nullptr;
        bool range = false;
        arg = skipwhite(arg + 1);
        if (*arg != ':') {
            idx1 = eval1(arg);
        }

        // Get the second variable from inside the [:].
        if (*arg == ':') {
            range = true;
            arg = skipwhite(arg + 1);
            if (*arg != ']') {
                idx2 = eval1(arg);
            }
        }

        // Check for the ']'.
        if (*arg != ']') {
            throw "Missing ']'";
        }
        arg++;

        if (range) {
            return factory->create<IndexRangeNode>(what, idx1, idx2);
        } else {
            return factory->create<IndexNode>(what, idx1);
        }
    }
}
