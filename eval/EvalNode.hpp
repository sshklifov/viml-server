#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <cassert>

struct EvalNode {
    virtual ~EvalNode() {}
    virtual std::string toString() = 0;
};

struct EvalFactory {
    EvalFactory() : topLevel(nullptr) {}
    EvalFactory(const EvalFactory&) = delete;
    EvalFactory(EvalFactory&&) = delete;

    ~EvalFactory() {
        for (EvalNode* node : allocatedNodes) {
            delete node;
        }
    }

    template <typename T, typename ... Args>
    T* create(Args&&... args) {
        static_assert(std::is_base_of<EvalNode, T>::value, "Bad template argument");

        T* res = new T(std::forward<Args>(args)...);
        allocatedNodes.push_back(res);
        return res;
    }

    void setTopLevel(EvalNode* node) {
        topLevel = node;
    }

    EvalNode* getTopLevel() { return topLevel; }

private:
    std::vector<EvalNode*> allocatedNodes;
    EvalNode* topLevel;
};

struct TernaryNode : public EvalNode {
    TernaryNode(EvalNode* cond, EvalNode* left, EvalNode* right) : cond(cond), left(left), right(right) {}

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
    EvalNode* cond;
    EvalNode* left;
    EvalNode* right;
};

struct LogicOpNode : public EvalNode {
    enum Type {AND, OR};

    LogicOpNode(EvalNode* lhs, EvalNode* rhs, Type op) : lhs(lhs), rhs(rhs), op(op) {}

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
    EvalNode* lhs;
    EvalNode* rhs;
    Type op;
};

struct InfixOpNode : public EvalNode {
    InfixOpNode(EvalNode* lhs, EvalNode* rhs, const char* op) : lhs(lhs), rhs(rhs), op(op) {}

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
    EvalNode* lhs;
    EvalNode* rhs;
    const char* op;
};

struct PrefixOpNode : public EvalNode {
    PrefixOpNode(EvalNode* val, const char* op) : val(val), op(op) {}

    std::string toString() override {
        std::string res;
        res += op;
        res += val->toString();
        return res;
    }

private:
    EvalNode* val;
    const char* op;
};

struct IndexNode : public EvalNode {
    IndexNode(EvalNode* what, EvalNode* index) : what(what), index(index) {}

    std::string toString() override {
        std::string res;
        res += what->toString();
        res += "[";
        res += index->toString();
        res += "]";
        return res;
    }

private:
    EvalNode* what;
    EvalNode* index;
};

struct IndexRangeNode : public EvalNode {
    IndexRangeNode(EvalNode* what, EvalNode* from, EvalNode* to) :
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
    EvalNode* what;
    EvalNode* from;
    EvalNode* to;
};

struct InvokeNode : public EvalNode {
    InvokeNode(EvalNode* fun, std::vector<EvalNode*> args) : fun(fun), args(std::move(args)) {}

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
    EvalNode* fun;
    std::vector<EvalNode*> args;
};

struct TokenNode : public EvalNode {
    enum Type {STRING, NUMBER, FLOAT, BLOB, OPTION, REGISTER, ENV, AUTOLOAD, VA, ID};

    TokenNode(std::string tok, Type type) : tok(std::move(tok)), type(type) {}

    std::string toString() override {
        return tok;
    }

private:
    std::string tok;
    Type type;
};

struct ListNode : public EvalNode {
    ListNode(std::vector<EvalNode*> elems) : elems(std::move(elems)) {}

    std::string toString() override {
        std::string res = "[";
        if (!elems.empty()) {
            elems[0]->toString();
            for (int i = 1; i < elems.size(); ++i) {
                res += ", ";
                res += elems[i]->toString();
            }
        }
        res += "]";
        return res;
    }

private:
    std::vector<EvalNode*> elems;
};

struct DictNode : public EvalNode {
    struct Pair {
        Pair() = default;
        Pair(EvalNode* key, EvalNode* value) : key(key), value(value) {}

        EvalNode* key;
        EvalNode* value;
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

struct NestedNode : public EvalNode {
    NestedNode(EvalNode* expr) : expr(expr) {}

    std::string toString() override {
        std::string res = "(";
        res += expr->toString();
        res += ")";
        return res;
    }

private:
    EvalNode* expr;
};

struct LambdaNode : public EvalNode {
    LambdaNode(std::vector<std::string> args, EvalNode* body) : args(std::move(args)), body(body) {}

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
    EvalNode* body;
};
