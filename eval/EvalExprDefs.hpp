#pragma once

#include <EvalExpr.hpp>

#include <vector>

struct TernaryNode : public EvalExpr {
    TernaryNode(EvalExpr* cond, EvalExpr* left, EvalExpr* right) : cond(cond), left(left), right(right) {}

    std::string toString() override {
        std::string res;
        res += cond->toString();
        res += " ? ";
        res += left->toString();
        res += " : ";
        res += right->toString();
        return res;
    }

private:
    EvalExpr* cond;
    EvalExpr* left;
    EvalExpr* right;
};

struct LogicOpNode : public EvalExpr {
    enum Type {AND, OR};

    LogicOpNode(EvalExpr* lhs, EvalExpr* rhs, Type op) : lhs(lhs), rhs(rhs), op(op) {}

    std::string toString() override {
        std::string res;
        res += lhs->toString();
        res += ' ';
        res += (op==AND ? "&&" : "||");
        res += ' ';
        res += rhs->toString();
        return res;
    }

private:
    EvalExpr* lhs;
    EvalExpr* rhs;
    Type op;
};

struct InfixOpNode : public EvalExpr {
    InfixOpNode(EvalExpr* lhs, EvalExpr* rhs, const char* op) : lhs(lhs), rhs(rhs), op(op) {}

    std::string toString() override {
        std::string res;
        res += lhs->toString();
        res += ' ';
        res += op;
        res += ' ';
        res += rhs->toString();
        return res;
    }

private:
    EvalExpr* lhs;
    EvalExpr* rhs;
    const char* op;
};

struct PrefixOpNode : public EvalExpr {
    PrefixOpNode(EvalExpr* val, const char* op) : val(val), op(op) {}

    std::string toString() override {
        std::string res;
        res += op;
        res += val->toString();
        return res;
    }

private:
    EvalExpr* val;
    const char* op;
};

struct IndexNode : public EvalExpr {
    IndexNode(EvalExpr* what, EvalExpr* index) : what(what), index(index) {}

    std::string toString() override {
        std::string res;
        res += what->toString();
        res += "[";
        res += index->toString();
        res += "]";
        return res;
    }

private:
    EvalExpr* what;
    EvalExpr* index;
};

struct IndexRangeNode : public EvalExpr {
    IndexRangeNode(EvalExpr* what, EvalExpr* from, EvalExpr* to) :
        what(what), from(from), to(to) {}

    std::string toString() override {
        std::string res;
        res += what->toString();
        res += "[";
        if (from) {
            res += from->toString();
        }
        res += ":";
        if (to) {
            res += to->toString();
        }
        res += "]";
        return res;
    }

private:
    EvalExpr* what;
    EvalExpr* from;
    EvalExpr* to;
};

struct InvokeNode : public EvalExpr {
    InvokeNode(EvalExpr* fun, std::vector<EvalExpr*> args) : fun(fun), args(std::move(args)) {}

    std::string toString() override {
        std::string res = fun->toString();

        res += "(";
        if (!args.empty()) {
            res += args[0]->toString();
            for (int i = 1; i < args.size(); ++i) {
                res += ", ";
                res += args[i]->toString();
            }
        }
        res += ")";

        return res;
    }

private:
    EvalExpr* fun;
    std::vector<EvalExpr*> args;
};

struct TokenNode : public EvalExpr {
    enum Type {STRING, NUMBER, FLOAT, BLOB, OPTION, REGISTER, ENV, AUTOLOAD, VA, ID};

    TokenNode(std::string tok, Type type) : tok(std::move(tok)), type(type) {}

    std::string toString() override {
        return tok;
    }

private:
    std::string tok;
    Type type;
};

struct ListNode : public EvalExpr {
    ListNode(std::vector<EvalExpr*> elems) : elems(std::move(elems)) {}

    std::string toString() override {
        std::string res = "[";
        if (!elems.empty()) {
            res += elems[0]->toString();
            for (int i = 1; i < elems.size(); ++i) {
                res += ", ";
                res += elems[i]->toString();
            }
        }
        res += "]";
        return res;
    }

private:
    std::vector<EvalExpr*> elems;
};

struct DictNode : public EvalExpr {
    struct Pair {
        Pair() = default;
        Pair(EvalExpr* key, EvalExpr* value) : key(key), value(value) {}

        EvalExpr* key;
        EvalExpr* value;
    };

    DictNode(std::vector<Pair> entries) : entries(std::move(entries)) {}

    std::string toString() override {
        std::string res = "{";
        if (!entries.empty()) {
            res += entries[0].key->toString();
                res += ": ";
            res += entries[0].value->toString();
            for (int i = 1; i < entries.size(); ++i) {
                res += ", ";
                res += entries[i].key->toString();
                res += ": ";
                res += entries[i].value->toString();
            }
        }
        res += "}";
        return res;
    }

private:
    std::vector<Pair> entries;
};

struct NestedNode : public EvalExpr {
    NestedNode(EvalExpr* expr) : expr(expr) {}

    std::string toString() override {
        std::string res = "(";
        res += expr->toString();
        res += ")";
        return res;
    }

private:
    EvalExpr* expr;
};

struct LambdaNode : public EvalExpr {
    LambdaNode(std::vector<std::string> args, EvalExpr* body) : args(std::move(args)), body(body) {}

    std::string toString() override {
        std::string res = "{";
        if (!args.empty()) {
            res += args[0];
            for (int i = 1; i < args.size(); ++i) {
                res += ", ";
                res += args[i];
            }
            res += " ";
        }
        res += "-> ";
        res += body->toString();
        return res;
    }

private:
    std::vector<std::string> args;
    EvalExpr* body;
};
