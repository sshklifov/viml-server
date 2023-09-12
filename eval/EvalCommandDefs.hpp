#pragma once

#include "EvalCommand.hpp"
#include "EvalExpr.hpp"

#include <vector>

enum class LetOp { NONE, PLUS, MINUS, MULT, DIV, MOD, DOT, DOT2};

struct LetCommand : public EvalCommand {
};

struct LetVar : public LetCommand {
    LetVar(FStr id, EvalExpr* expr, LetOp op = LetOp::NONE) :
        id(std::move(id)), expr(expr), op(op) {}

    FStr id;
    EvalExpr* expr;
    LetOp op;
};

struct LetElement : public LetCommand {
    LetElement(FStr id, EvalExpr* elem, EvalExpr* expr) :
        id(std::move(id)), elem(elem), expr(expr)  {}

    FStr id;
    EvalExpr* elem;
    EvalExpr* expr;
};

struct LetRange : public LetCommand {
    LetRange(FStr id, EvalExpr* from, EvalExpr* to, EvalExpr* expr) :
        id(std::move(id)), from(from), to(to), expr(expr) {}

    FStr id;
    EvalExpr* from;
    EvalExpr* to;
    EvalExpr* expr;
};

struct LetUnpack : public LetCommand {
    LetUnpack(std::vector<FStr> ids, EvalExpr* expr, LetOp op = LetOp::NONE) :
        ids(std::move(ids)), expr(expr), op(op) {}

    std::vector<FStr> ids;
    EvalExpr* expr;
    LetOp op;
};

struct LetRemainder : public LetCommand {
    LetRemainder(std::vector<FStr> ids, FStr rem, EvalExpr* expr, LetOp op = LetOp::NONE) :
        ids(std::move(ids)), rem(std::move(rem)), expr(expr), op(op) {}

    std::vector<FStr> ids;
    FStr rem;
    EvalExpr* expr;
    LetOp op;
};

struct LetPrint : public LetCommand {
    LetPrint() {}
    LetPrint(std::vector<FStr> ids) : ids(std::move(ids)) {}

    std::vector<FStr> ids;
};

struct Unlet : public EvalCommand {
    Unlet(std::vector<FStr> ids) : ids(std::move(ids)) {}

    std::vector<FStr> ids;
};

struct ConstCommand : public EvalCommand {
};

struct ConstVar : public ConstCommand {
    ConstVar(FStr id, EvalExpr* expr) : id(std::move(id)), expr(expr) {}

    FStr id;
    EvalExpr* expr;
};

struct ConstUnpack : public ConstCommand {
    ConstUnpack(std::vector<FStr> ids, EvalExpr* expr) :
        ids(std::move(ids)), expr(expr) {}

    std::vector<FStr> ids;
    EvalExpr* expr;
};

struct ConstRemainder : public ConstCommand {
    ConstRemainder(std::vector<FStr> ids, FStr rem, EvalExpr* expr) :
        ids(std::move(ids)), rem(std::move(rem)), expr(expr) {}

    std::vector<FStr> ids;
    FStr rem;
    EvalExpr* expr;
};

struct LockVar : public EvalCommand {
    LockVar(std::vector<FStr> ids) : ids(std::move(ids)), depth("2") {}
    LockVar(std::vector<FStr> ids, FStr depth) :
        ids(std::move(ids)), depth(std::move(depth)) {}

    std::vector<FStr> ids;
    FStr depth;
};

struct UnlockVar : public EvalCommand {
    UnlockVar(std::vector<FStr> ids) : ids(std::move(ids)), depth("2") {}
    UnlockVar(std::vector<FStr> ids, FStr depth) :
        ids(std::move(ids)), depth(std::move(depth)) {}

    std::vector<FStr> ids;
    FStr depth;
};

struct FunctionCommand : public EvalCommand {
};

struct FunctionPrint : public FunctionCommand {
    FunctionPrint() = default;
    FunctionPrint(FStr pat) : pat(std::move(pat)) {}

    FStr pat;
};

struct Function : public FunctionCommand {
    Function(FStr name, std::vector<FStr> args, int va = false) :
        name(std::move(name)), args(std::move(args)), variadic(va) {}

    FStr name;
    std::vector<FStr> args;
    bool variadic;
};

struct FunctionDict : public FunctionCommand {
    FunctionDict(FStr dict, FStr key, std::vector<FStr> args, int va = false) :
        dict(std::move(dict)), key(std::move(key)), args(std::move(args)), variadic(va) {}

    FStr dict;
    FStr key;
    std::vector<FStr> args;
    bool variadic;
};
