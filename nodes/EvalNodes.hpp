#pragma once

#include "BaseNode.hpp"

struct CallNode : public BaseNode {
    CallNode(const ExLexem& lexem) : BaseNode(lexem) { expr = nullptr; }

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;
        try {
            expr = eval8(p, f);
            if (expr->getId() != EvalExpr::invoke) {
                rep.error("Does not call a function");
            }
            return p;
        } catch (msg& m) {
            rep.error(m);
            return nullptr;
        }
    }

    static const int id = CMD_call;

private:
    EvalExpr* expr;
};

struct ReturnNode : public BaseNode {
    ReturnNode(const ExLexem& lexem) : BaseNode(lexem) { expr = nullptr; }

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;
        if (ends_notrlcom(*p)) {
            return p;
        }
        try {
            expr = eval1(p, f);
            return p;
        } catch (msg& m) {
            rep.error(m);
            return nullptr;
        }
    }

    static const int id = CMD_call;

private:
    EvalExpr* expr;
};

struct EvalNode : public BaseNode {
    EvalNode(const ExLexem& lexem) : BaseNode(lexem) { expr = nullptr; }

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;
        try {
            expr = eval1(p, f);
            return p;
        } catch (msg& m) {
            rep.error(m);
            expr = nullptr;
            f.clear();
            return nullptr;
        }
    }

    static const int id = CMD_eval;

private:
    EvalExpr* expr;
};

struct ExecuteNode : public BaseNode {
    ExecuteNode(const ExLexem& lexem) : BaseNode(lexem) { expr = nullptr; }

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;
        try {
            for (;;) {
                if (ends_notrlcom(*p)) {
                    return p;
                }
                EvalExpr* next = eval1(p, f);
                expr = f.create<BinOpExpr>(expr, next, '.');
            }
        } catch (msg& m) {
            rep.error(m);
            expr = nullptr;
            f.clear();
            return nullptr;
        }
    }

    static const int id = CMD_call;

private:
    EvalExpr* expr;
};
