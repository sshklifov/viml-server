#pragma once

#include "EvalNode.hpp"
#include <ExConstants.hpp>

enum class LetOp { NONE, PLUS, MINUS, MULT, DIV, MOD, DOT, DOT2};

struct EvalCommand {
    virtual int getId() = 0;
};

struct LetCommand : public EvalCommand {
    int getId() override { return LET; }
};

struct LetVar : public LetCommand {
    LetVar(std::string id, EvalNode* expr, LetOp op = LetOp::NONE) :
        id(std::move(id)), expr(expr), op(op) {}

    std::string id;
    EvalNode* expr;
    LetOp op;
};

struct LetElement : public LetCommand {
    LetElement(std::string id, EvalNode* elem, EvalNode* expr) :
        id(std::move(id)), elem(elem), expr(expr)  {}

    std::string id;
    EvalNode* elem;
    EvalNode* expr;
};

struct LetRange : public LetCommand {
    LetRange(std::string id, EvalNode* from, EvalNode* to, EvalNode* expr) :
        id(std::move(id)), from(from), to(to), expr(expr) {}

    std::string id;
    EvalNode* from;
    EvalNode* to;
    EvalNode* expr;
};

struct LetUnpack : public LetCommand {
    LetUnpack(std::vector<std::string> ids, EvalNode* expr, LetOp op = LetOp::NONE) :
        ids(std::move(ids)), expr(expr), op(op) {}

    std::vector<std::string> ids;
    EvalNode* expr;
    LetOp op;
};

struct LetRemainder : public LetCommand {
    LetRemainder(std::vector<std::string> ids, std::string rem, EvalNode* expr, LetOp op = LetOp::NONE) :
        ids(std::move(ids)), rem(std::move(rem)), expr(expr), op(op) {}

    std::vector<std::string> ids;
    std::string rem;
    EvalNode* expr;
    LetOp op;
};

struct LetPrint : public LetCommand {
    LetPrint() {}
    LetPrint(std::vector<std::string> ids) : ids(std::move(ids)) {}

    std::vector<std::string> ids;
};

struct Unlet : public EvalCommand {
    Unlet(std::vector<std::string> ids) : ids(std::move(ids)) {}

    int getId() override { return UNLET; }

    std::vector<std::string> ids;
};

struct ConstCommand : public EvalCommand {
    int getId() override { return CONST; }
};

struct ConstVar : public ConstCommand {
    ConstVar(std::string id, EvalNode* expr) : id(std::move(id)), expr(expr) {}

    std::string id;
    EvalNode* expr;
};

struct ConstUnpack : public ConstCommand {
    ConstUnpack(std::vector<std::string> ids, EvalNode* expr) :
        ids(std::move(ids)), expr(expr) {}

    std::vector<std::string> ids;
    std::string rem;
    EvalNode* expr;
};

struct ConstRemainder : public ConstCommand {
    ConstRemainder(std::vector<std::string> ids, std::string rem, EvalNode* expr) :
        ids(std::move(ids)), rem(std::move(rem)), expr(expr) {}

    std::vector<std::string> ids;
    std::string rem;
    EvalNode* expr;
};

struct LockVar : public EvalCommand {
    LockVar(std::vector<std::string> ids) : ids(std::move(ids)), depth("2") {}
    LockVar(std::vector<std::string> ids, std::string depth) :
        ids(std::move(ids)), depth(std::move(depth)) {}

    int getId() override { return LOCKVAR; }

    std::vector<std::string> ids;
    std::string depth;
};

struct UnlockVar : public EvalCommand {
    UnlockVar(std::vector<std::string> ids) : ids(std::move(ids)), depth("2") {}
    UnlockVar(std::vector<std::string> ids, std::string depth) :
        ids(std::move(ids)), depth(std::move(depth)) {}

    int getId() override { return UNLOCKVAR; }

    std::vector<std::string> ids;
    std::string depth;
};

struct FunctionCommand : public EvalCommand {
    int getId() override { return FUNCTION; }
};

struct FunctionPrint : public FunctionCommand {
    FunctionPrint() = default;
    FunctionPrint(std::string pat) : pat(std::move(pat)) {}

    std::string pat;
};

struct Function : public FunctionCommand {
    Function(std::string name, std::vector<std::string> args) :
        name(std::move(name)), args(std::move(args)) {}

    std::string name;
    std::vector<std::string> args;
};

struct FunctionDict : public FunctionCommand {
    FunctionDict(std::string dict, std::string key, std::vector<std::string> args) :
        dict(std::move(dict)), key(std::move(key)), args(std::move(args)) {}

    std::string dict;
    std::string key;
    std::vector<std::string> args;
};
