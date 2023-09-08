#pragma once

#include <EvalExpr.hpp>

#include <string>
#include <vector>

struct TernaryNode : public EvalExpr {
    TernaryNode(EvalExpr* cond, EvalExpr* left, EvalExpr* right) : cond(cond), left(left), right(right) {}

    void appendStr(FStr& res) override {
        cond->appendStr(res);
        res += " ? ";
        left->appendStr(res);
        res += " : ";
        right->appendStr(res);
    }

private:
    EvalExpr* cond;
    EvalExpr* left;
    EvalExpr* right;
};

struct LogicOpNode : public EvalExpr {
    enum Type {AND, OR};

    LogicOpNode(EvalExpr* lhs, EvalExpr* rhs, Type op) : lhs(lhs), rhs(rhs), op(op) {}

    void appendStr(FStr& res) override {
        lhs->appendStr(res);
        if (op == AND) {
            res += " && ";
        } else {
            res += " || ";
        }
        rhs->appendStr(res);
    }

private:
    EvalExpr* lhs;
    EvalExpr* rhs;
    Type op;
};

struct InfixOpNode : public EvalExpr {
    InfixOpNode(EvalExpr* lhs, EvalExpr* rhs, const char* op) : lhs(lhs), rhs(rhs), op(op) {}

    void appendStr(FStr& res) override {
        lhs->appendStr(res);
        res.appendf(" {} ", op);
        rhs->appendStr(res);
    }

private:
    EvalExpr* lhs;
    EvalExpr* rhs;
    const char* op;
};

struct PrefixOpNode : public EvalExpr {
    PrefixOpNode(EvalExpr* val, const char* op) : val(val), op(op) {}

    void appendStr(FStr& res) override {
        res += op;
        val->appendStr(res);
    }

private:
    EvalExpr* val;
    const char* op;
};

struct IndexNode : public EvalExpr {
    IndexNode(EvalExpr* what, EvalExpr* index) : what(what), index(index) {}

    void appendStr(FStr& res) override {
        what->appendStr(res);
        res += '[';
        index->appendStr(res);
        res += ']';
    }

private:
    EvalExpr* what;
    EvalExpr* index;
};

struct IndexRangeNode : public EvalExpr {
    IndexRangeNode(EvalExpr* what, EvalExpr* from, EvalExpr* to) :
        what(what), from(from), to(to) {}

    void appendStr(FStr& res) override {
        what->appendStr(res);
        res += '[';
        if (from) {
            from->appendStr(res);
        }
        res += ':';
        if (to) {
            to->appendStr(res);
        }
        res += ']';
    }

private:
    EvalExpr* what;
    EvalExpr* from;
    EvalExpr* to;
};

struct InvokeNode : public EvalExpr {
    InvokeNode(EvalExpr* fun, std::vector<EvalExpr*> args) : fun(fun), args(std::move(args)) {}

    void appendStr(FStr& res) override {
        fun->appendStr(res);
        res += '(';
        if (!args.empty()) {
            args[0]->appendStr(res);
            for (int i = 1; i < args.size(); ++i) {
                res += ", ";
                args[i]->appendStr(res);
            }
        }
        res += ")";
    }

private:
    EvalExpr* fun;
    std::vector<EvalExpr*> args;
};

struct TokenNode : public EvalExpr {
    enum Type {STRING, NUMBER, FLOAT, BLOB, OPTION, REGISTER, ENV, AUTOLOAD, VA, ID};

    TokenNode(std::string tok, Type type) : tok(std::move(tok)), type(type) {}

    void appendStr(FStr& res) override {
        res += tok.c_str();
    }

private:
    std::string tok;
    Type type;
};

struct ListNode : public EvalExpr {
    ListNode(std::vector<EvalExpr*> elems) : elems(std::move(elems)) {}

    void appendStr(FStr& res) override {
        res += "[";
        if (!elems.empty()) {
            elems[0]->appendStr(res);
            for (int i = 1; i < elems.size(); ++i) {
                res += ", ";
                elems[i]->appendStr(res);
            }
        }
        res += "]";
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

    void appendStr(FStr& res) override {
        res += "{";
        if (!entries.empty()) {
            entries[0].key->appendStr(res);
            res += ": ";
            entries[0].value->appendStr(res);
            for (int i = 1; i < entries.size(); ++i) {
                res += ", ";
                entries[i].key->appendStr(res);
                res += ": ";
                entries[i].value->appendStr(res);
            }
        }
        res += "}";
    }

private:
    std::vector<Pair> entries;
};

struct NestedNode : public EvalExpr {
    NestedNode(EvalExpr* expr) : expr(expr) {}

    void appendStr(FStr& res) override {
        res += "(";
        expr->appendStr(res);
        res += ")";
    }

private:
    EvalExpr* expr;
};

struct LambdaNode : public EvalExpr {
    LambdaNode(std::vector<std::string> args, EvalExpr* body) : args(std::move(args)), body(body) {}

    void appendStr(FStr& res) override {
        res += "{";
        if (!args.empty()) {
            res += args[0].c_str();
            for (int i = 1; i < args.size(); ++i) {
                res += ", ";
                res += args[i].c_str();
            }
            res += " ";
        }
        res += "-> ";
        body->appendStr(res);
    }

private:
    std::vector<std::string> args;
    EvalExpr* body;
};
