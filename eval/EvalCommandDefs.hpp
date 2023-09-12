#pragma once

#include "EvalCommand.hpp"
#include "EvalExpr.hpp"

#include <vector>

enum class LetOp { EQUAL, PLUS, MINUS, MULT, DIV, MOD, DOT, CONCAT};

struct LetCommand : public EvalCommand {
};

struct LetVar : public LetCommand {
    LetVar(FStr id, EvalExpr* expr, LetOp op) :
        id(std::move(id)), expr(expr), op(op) {}

    FStr id;
    EvalExpr* expr;
    LetOp op;
};

struct LetElement : public LetCommand {
    LetElement(FStr id, EvalExpr* elem, EvalExpr* expr, LetOp op) :
        id(std::move(id)), elem(elem), expr(expr), op(op)  {}

    FStr id;
    EvalExpr* elem;
    EvalExpr* expr;
    LetOp op;
};

struct LetRange : public LetCommand {
    LetRange(FStr id, EvalExpr* from, EvalExpr* to, EvalExpr* expr, LetOp op) :
        id(std::move(id)), from(from), to(to), expr(expr) {}

    FStr id;
    EvalExpr* from;
    EvalExpr* to;
    EvalExpr* expr;
    LetOp op;
};

struct LetUnpack : public LetCommand {
    LetUnpack(std::vector<FStr> ids, EvalExpr* expr, LetOp op) :
        ids(std::move(ids)), expr(expr), op(op) {}

    std::vector<FStr> ids;
    EvalExpr* expr;
    LetOp op;
};

struct LetRemainder : public LetCommand {
    LetRemainder(std::vector<FStr> ids, FStr rem, EvalExpr* expr, LetOp op) :
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

struct UnletCommand : public EvalCommand {
};

struct Unlet : public UnletCommand {
    Unlet(std::vector<FStr> ids) : ids(std::move(ids)) {}

    std::vector<FStr> ids;
};

struct UnletElement : public UnletCommand {
    UnletElement(FStr id, EvalExpr* elem) :
        id(std::move(id)), elem(elem) {}

    FStr id;
    EvalExpr* elem;
};

struct UnletRange : public UnletCommand {
    UnletRange(FStr id, EvalExpr* from, EvalExpr* to) :
        id(std::move(id)), from(from), to(to) {}

    FStr id;
    EvalExpr* from;
    EvalExpr* to;
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
    Function(FStr name, std::vector<FStr> args, int va, std::vector<FStr> attrs) :
        name(std::move(name)), args(std::move(args)), variadic(va), attrs(std::move(attrs)) {}

    FStr name;
    std::vector<FStr> args;
    bool variadic;
    std::vector<FStr> attrs;
};

struct FunctionDict : public FunctionCommand {
    FunctionDict(FStr dict, FStr key, std::vector<FStr> args, int va, std::vector<FStr> attrs) :
        dict(std::move(dict)), key(std::move(key)), args(std::move(args)), variadic(va), attrs(std::move(attrs)) {}

    FStr dict;
    FStr key;
    std::vector<FStr> args;
    bool variadic;
    std::vector<FStr> attrs;
};

struct If : public EvalCommand {
    If(EvalExpr* expr) : expr(expr) {}

    EvalExpr* expr;
};

struct ElseIf : public EvalCommand {
    ElseIf(EvalExpr* expr) : expr(expr) {}

    EvalExpr* expr;
};

struct Call : public EvalCommand {
    Call(EvalExpr* expr) : expr(expr) {}

    EvalExpr* expr;
};

struct Return : public EvalCommand {
    Return(EvalExpr* expr) : expr(expr) {}

    EvalExpr* expr;
};
