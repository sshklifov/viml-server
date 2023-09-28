#pragma once

#include <EvalExpr.hpp>
#include <Vector.hpp>
#include <FStr.hpp>

struct TernaryExpr : public EvalExpr {
    TernaryExpr(EvalExpr* cond, EvalExpr* left, EvalExpr* right) : cond(cond), left(left), right(right) {}

private:
    EvalExpr* cond;
    EvalExpr* left;
    EvalExpr* right;
};

struct LogicOpExpr : public EvalExpr {
    enum Type {AND, OR};

    LogicOpExpr(EvalExpr* lhs, EvalExpr* rhs, Type op) : lhs(lhs), rhs(rhs), op(op) {}

private:
    EvalExpr* lhs;
    EvalExpr* rhs;
    Type op;
};

struct CmpOpExpr : public EvalExpr {
    CmpOpExpr(EvalExpr* lhs, EvalExpr* rhs, int enumOp, char mod = 0) :
        lhs(lhs), rhs(rhs), enumOp(enumOp), mod(mod) {}

private:
    EvalExpr* lhs;
    EvalExpr* rhs;
    int enumOp;
    char mod;
};

struct ArithOpExpr : public EvalExpr {
    ArithOpExpr(EvalExpr* lhs, EvalExpr* rhs, char op) :
        lhs(lhs), rhs(rhs), op(op) {}

private:
    EvalExpr* lhs;
    EvalExpr* rhs;
    char op;
};

struct PrefixOpExpr : public EvalExpr {
    PrefixOpExpr(EvalExpr* val, char op) : val(val), op(op) {}

private:
    EvalExpr* val;
    char op;
};

struct IndexExpr : public EvalExpr {
    IndexExpr(EvalExpr* what, EvalExpr* index) : what(what), index(index) {}

private:
    EvalExpr* what;
    EvalExpr* index;
};

struct IndexRangeExpr : public EvalExpr {
    IndexRangeExpr(EvalExpr* what, EvalExpr* from, EvalExpr* to) :
        what(what), from(from), to(to) {}

private:
    EvalExpr* what;
    EvalExpr* from;
    EvalExpr* to;
};

struct InvokeExpr : public EvalExpr {
    InvokeExpr(EvalExpr* name, Vector<EvalExpr*> args) : name(name), args(std::move(args)) {}

private:
    EvalExpr* name;
    Vector<EvalExpr*> args;
};

struct NameExpr : public EvalExpr {
    NameExpr(EvalExpr* name) : name(name) {}

private:
    EvalExpr* name;
};

struct LiteralExpr : public EvalExpr {
    LiteralExpr(int type, FStr tok) : tok(std::move(tok)), type(type) {}

private:
    int type;
    FStr tok;
};

struct ListExpr : public EvalExpr {
    ListExpr(Vector<EvalExpr*> elems) : elems(std::move(elems)) {}

private:
    Vector<EvalExpr*> elems;
};

struct DictExpr : public EvalExpr {
    struct Pair {
        Pair() = default;
        Pair(EvalExpr* key, EvalExpr* value) : key(key), value(value) {}

        EvalExpr* key;
        EvalExpr* value;
    };

    DictExpr(Vector<Pair> entries) : entries(std::move(entries)) {}

private:
    Vector<Pair> entries;
};

struct NestedExpr : public EvalExpr {
    NestedExpr(EvalExpr* expr) : expr(expr) {}

private:
    EvalExpr* expr;
};

struct LambdaExpr : public EvalExpr {
    LambdaExpr(Vector<FStr> args, EvalExpr* body) : args(std::move(args)), body(body) {}

private:
    Vector<FStr> args;
    EvalExpr* body;
};
