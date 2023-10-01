#pragma once

#include "BaseNode.hpp"
#include "SkipFuncs.hpp"

struct LetNode : public BaseNode {
    LetNode(const ExLexem& lexem) : BaseNode(lexem) { reset(); }

    void reset() {
        f.clear();
        vars.clear();
        semicolon = 0;
        op = 0;
        expr = nullptr;
    }

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;

        /// ":let" list all variable values
        if (ends_excmd(*p)) {
            return p;
        }
        try {
            int first_char = *p;
            vars = get_var_list(p, 1, semicolon, f);
            p = skipwhite(p);

            if (!ascii_haschar("=+-*/%.", *p)) {
                /// ":let var1 var2" list variable values
                if (first_char == '[')  {
                    throw msg(p, "Expecting operator");
                } else {
                    while (!ends_excmd(*p)) {
                        vars.emplace(get_var_one(p, 1, f));
                    }
                    return p;
                }
            }

            op = *p;
            if (p[0] == '=') {
                p++; // "="
            } else if (p[1] == '=') {
                p += 2; // "+=", "-=", "*=", "/=", ".="
            } else if (*p == '.' && p[1] == '.' && p[2] == '=') {
                p += 3; // "..="
            } else {
                throw msg(p, "Invalid :let operator");
            }

            p = skipwhite(p);
            expr = eval1(p, f);
            return p;
        } catch (msg& m) {
            rep.error(m);
            reset();
            return nullptr;
        }
    }

    static const int id = CMD_let;

private:
    Vector<EvalExpr*> vars;
    int semicolon;
    int op;
    EvalExpr* expr;
};

struct ConstNode : public BaseNode {
    ConstNode(const ExLexem& lexem) : BaseNode(lexem) { expr = nullptr; }

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;

        /// ":const" list all variable values
        if (ends_excmd(*p)) {
            return p;
        }
        try {
            int first_char = *p;
            vars = get_var_list(p, 0, semicolon, f);
            p = skipwhite(p);
            if (*p != '=') {
                /// ":const var1 var2" list variable values
                if (first_char == '[') {
                    throw msg(p, "Expecting '='");
                } else {
                    while (!ends_excmd(*p)) {
                        vars.emplace(get_var_one(p, 0, f));
                    }
                    return p;
                }
            }
            /// ":const [var1, var2] = expr" unpack list and lock variables.
            /// ":const [name, ..., ; lastname] = expr" unpack list and lock variables.
            /// ":const var = expr" set and lock variable
            p = skipwhite(p + 1);
            expr = eval1(p, f);
            return p;
        } catch (msg& m) {
            rep.error(m);
            f.clear();
            vars.clear();
            expr = nullptr;
            return nullptr;
        }
    }

    static const int id = CMD_const;

private:
    Vector<EvalExpr*> vars;
    int semicolon;
    EvalExpr* expr;
};

struct UnletNode : public BaseNode {
    UnletNode(ExLexem& lexem) : BaseNode(lexem) {}

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;

        try {
            do {
                if (*p == '@' || *p == '&') {
                    throw msg(p, "Cannot remove options or registers");
                } else if (*p == '$') {
                    vars.emplace(get_var_special(p, f));
                } else {
                    vars.emplace(get_var_indexed(p, f));
                }
                p = skipwhite(p);
            } while (!ends_excmd(*p));
            return p;
        } catch (msg& m) {
            rep.error(m);
            f.clear();
            vars.clear();
            return nullptr;
        }
    }

    static const int id = CMD_unlet;

private:
    Vector<EvalExpr*> vars;
};

struct LockvarNode : public BaseNode {
    LockvarNode(ExLexem& lexem) : BaseNode(lexem) {}

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;

        try {
            if (*p >= '0' && *p <= '9') {
                VarType type = VAR_UNKNOWN;
                const char* q = skip_numerical(p, type);
                depth = FStr(p, q);
                p = skipwhite(q);
            }
            do {
                if (*p == '@' || *p == '&') {
                    throw msg(p, "Cannot remove options or registers");
                } else if (*p == '$') {
                    vars.emplace(get_var_special(p, f));
                } else {
                    vars.emplace(get_var_indexed(p, f));
                }
            } while (!ends_excmd(*p));
            return p;
        } catch (msg& m) {
            rep.error(m);
            f.clear();
            vars.clear();
            return nullptr;
        }
    }

    static const int id = CMD_lockvar;

private:
    FStr depth;
    Vector<EvalExpr*> vars;
};
