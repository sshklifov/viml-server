#pragma once

#include <EvalExpr.hpp>
#include <Vector.hpp>

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

struct CmpOpNode : public EvalExpr {
    CmpOpNode(EvalExpr* lhs, EvalExpr* rhs, int enumOp, char mod = 0) :
        lhs(lhs), rhs(rhs), enumOp(enumOp), mod(mod) {}

    void appendStr(FStr& res) override {
        assert(false); // TODO
        lhs->appendStr(res);
        // res.appendf(" {} ", op);
        rhs->appendStr(res);
    }

private:
    EvalExpr* lhs;
    EvalExpr* rhs;
    int enumOp;
    char mod;
};

struct ArithOpNode : public EvalExpr {
    ArithOpNode(EvalExpr* lhs, EvalExpr* rhs, char op) :
        lhs(lhs), rhs(rhs), op(op) {}

    void appendStr(FStr& res) override {
        lhs->appendStr(res);
        res.appendf(" {} ", op);
        rhs->appendStr(res);
    }

private:
    EvalExpr* lhs;
    EvalExpr* rhs;
    char op;
};

struct PrefixOpNode : public EvalExpr {
    PrefixOpNode(EvalExpr* val, char op) : val(val), op(op) {}

    void appendStr(FStr& res) override {
        res += op;
        val->appendStr(res);
    }

private:
    EvalExpr* val;
    char op;
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
    InvokeNode(EvalExpr* name, Vector<EvalExpr*> args) : name(name), args(std::move(args)) {}

    void appendStr(FStr& res) override {
        name->appendStr(res);
        res += '(';
        if (!args.empty()) {
            args[0]->appendStr(res);
            for (int i = 1; i < args.count(); ++i) {
                res += ", ";
                args[i]->appendStr(res);
            }
        }
        res += ")";
    }

private:
    EvalExpr* name;
    Vector<EvalExpr*> args;
};

struct NameNode : public EvalExpr {
    NameNode(EvalExpr* name) : name(name) {}

    void appendStr(FStr& res) override {
        name->appendStr(res);
    }

private:
    EvalExpr* name;
};

struct TokenNode : public EvalExpr {
    enum Type {STRING, NUMBER, FLOAT, BLOB, OPTION, REGISTER, ENV, AUTOLOAD, SID, DICT, ID};

    TokenNode(FStr tok, Type type) : tok(std::move(tok)), type(type) {}

    void appendStr(FStr& res) override {
        res += tok;
    }

private:
    FStr tok;
    Type type;
}; // TODO superseded by BaseNode

struct BaseNode : public EvalExpr {
    BaseNode(int type, FStr tok) : tok(std::move(tok)), type(type) {}

    void appendStr(FStr& res) override {
        res += tok;
    }

private:
    int type;
    FStr tok;
};

struct ListNode : public EvalExpr {
    ListNode(Vector<EvalExpr*> elems) : elems(std::move(elems)) {}

    void appendStr(FStr& res) override {
        res += "[";
        if (!elems.empty()) {
            elems[0]->appendStr(res);
            for (int i = 1; i < elems.count(); ++i) {
                res += ", ";
                elems[i]->appendStr(res);
            }
        }
        res += "]";
    }

private:
    Vector<EvalExpr*> elems;
};

struct DictNode : public EvalExpr {
    struct Pair {
        Pair() = default;
        Pair(EvalExpr* key, EvalExpr* value) : key(key), value(value) {}

        bool operator<(const Pair& rhs) {
            // TODO
            return this < &rhs;
        }

        EvalExpr* key;
        EvalExpr* value;
    };

    DictNode(Vector<Pair> entries) : entries(std::move(entries)) {}

    void appendStr(FStr& res) override {
        res += "{";
        if (!entries.empty()) {
            entries[0].key->appendStr(res);
            res += ": ";
            entries[0].value->appendStr(res);
            for (int i = 1; i < entries.count(); ++i) {
                res += ", ";
                entries[i].key->appendStr(res);
                res += ": ";
                entries[i].value->appendStr(res);
            }
        }
        res += "}";
    }

private:
    Vector<Pair> entries;
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
    LambdaNode(Vector<FStr> args, EvalExpr* body) : args(std::move(args)), body(body) {}

    void appendStr(FStr& res) override {
        res += "{";
        if (!args.empty()) {
            res += args[0];
            for (int i = 1; i < args.count(); ++i) {
                res += ", ";
                res += args[i];
            }
            res += " ";
        }
        res += "-> ";
        body->appendStr(res);
    }

private:
    Vector<FStr> args;
    EvalExpr* body;
};
