#pragma once

#include "BaseNode.hpp"
#include <EvalUtil.hpp>

struct WhileNode : public GroupNode {
    WhileNode(const ExLexem& lexem) : GroupNode(lexem), cond(nullptr) {}

    const char* parseArgs(BoundReporter& rep) override {
        try {
            const char* p = lex.qargs;
            cond = eval1(p, f);
            return p;
        } catch (msg& m) {
            rep.error(m);
            return nullptr;
        }
    }

    static const int id = CMD_while;

private:
    EvalExpr* cond;
};

struct ForNode : public GroupNode {
    ForNode(const ExLexem& lexem) : GroupNode(lexem) { reset(); }

    void reset() {
        f.clear();
        semicolon = 0;
        vars.clear();
        elems = nullptr;
    }

    const char* parseArgs(BoundReporter& rep) override {
        try {
            const char* arg = lex.qargs;
            vars = get_var_list(arg, 1, semicolon, f);
            arg = skipwhite(arg);
            if (arg[0] != 'i' || arg[1] != 'n' || !ascii_iswhite(arg[2])) {
                throw msg(arg, "Missing \"in\" after :for");
            }
            arg = skipwhite(arg + 2);
            elems = eval1(arg, f);
            return arg;
        } catch (msg& m) {
            rep.error(m);
            reset();
            return nullptr;
        }
    }

    static const int id = CMD_for;

private:
    int semicolon;
    Vector<EvalExpr*> vars;
    EvalExpr* elems;
};
