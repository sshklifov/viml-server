#pragma once

#include <EvalExpr.hpp>
#include <Vector.hpp>
#include <FStr.hpp>

struct TernaryExpr : public EvalExpr {
    TernaryExpr(EvalExpr* cond, EvalExpr* left, EvalExpr* right) : cond(cond), left(left), right(right) {}

    int getId() override { return expr_ternary; }

    EvalExpr* cond;
    EvalExpr* left;
    EvalExpr* right;
};

struct LogicOpExpr : public EvalExpr {
    enum Type {AND, OR};

    LogicOpExpr(EvalExpr* lhs, EvalExpr* rhs, Type op) : lhs(lhs), rhs(rhs), op(op) {}

    int getId() override { return expr_logic; }

    EvalExpr* lhs;
    EvalExpr* rhs;
    Type op;
};

struct CmpOpExpr : public EvalExpr {
    CmpOpExpr(EvalExpr* lhs, EvalExpr* rhs, int enumOp, char mod = 0) :
        lhs(lhs), rhs(rhs), enumOp(enumOp), mod(mod) {}

    int getId() override { return expr_compare; }

    EvalExpr* lhs;
    EvalExpr* rhs;
    int enumOp;
    char mod;
};

struct BinOpExpr : public EvalExpr {
    BinOpExpr(EvalExpr* lhs, EvalExpr* rhs, char op) :
        lhs(lhs), rhs(rhs), op(op) {}

    int getId() override { return expr_arith; }

    EvalExpr* lhs;
    EvalExpr* rhs;
    char op;
};

struct PrefixOpExpr : public EvalExpr {
    PrefixOpExpr(EvalExpr* val, char op) : val(val), op(op) {}

    int getId() override { return expr_prefix; }

    EvalExpr* val;
    char op;
};

struct IndexExpr : public EvalExpr {
    IndexExpr(EvalExpr* what, EvalExpr* index) : what(what), index(index) {}

    int getId() override { return expr_index; }

    EvalExpr* what;
    EvalExpr* index;
};

struct IndexRangeExpr : public EvalExpr {
    IndexRangeExpr(EvalExpr* what, EvalExpr* from, EvalExpr* to) :
        what(what), from(from), to(to) {}

    int getId() override { return expr_index2; }

    EvalExpr* what;
    EvalExpr* from;
    EvalExpr* to;
};

struct InvokeExpr : public EvalExpr {
    InvokeExpr(EvalExpr* name, Vector<EvalExpr*> args) : name(name), args(std::move(args)) {}

    int getId() override { return expr_invoke; }

    EvalExpr* name;
    Vector<EvalExpr*> args;
};

struct SymbolExpr : public EvalExpr {
    SymbolExpr(EvalExpr* name, const char* s, int n) : name(name), begin(s), end(s + n) {}
    SymbolExpr(EvalExpr* name, const char* begin, const char* end) : name(name), begin(begin), end(end) {}

    int getId() override { return expr_symbol; }

    EvalExpr* name;
    const char* begin;
    const char* end;
};

struct LiteralExpr : public EvalExpr {
    LiteralExpr(int type, FStr tok) : lit(std::move(tok)), type(type) {}
    LiteralExpr(int type, const char* begin, const char* end) : lit(begin, end), type(type) {}
    LiteralExpr(int type, const char* lit, int len) : lit(lit, len), type(type) {}

    int getId() override { return expr_literal; }

    int type;
    FStr lit;
};

struct ListExpr : public EvalExpr {
    ListExpr(Vector<EvalExpr*> elems) : elems(std::move(elems)) {}

    int getId() override { return expr_list; }

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

    int getId() override { return expr_dict; }

    Vector<Pair> entries;
};

struct NestedExpr : public EvalExpr {
    NestedExpr(EvalExpr* expr) : expr(expr) {}

    int getId() override { return expr_nest; }

    EvalExpr* expr;
};

struct LambdaExpr : public EvalExpr {
    LambdaExpr(Vector<FStr> args, EvalExpr* body) : args(std::move(args)), body(body) {}

    int getId() override { return expr_lambda; }

    Vector<FStr> args;
    EvalExpr* body;
};
