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
    struct None {};
    struct Single {};
    struct Left {};
    struct Right {};
    struct Double {};

    IndexNode(None, EvalNode* val) :
        val(val), type(NONE), from(nullptr), to(nullptr) {}

    IndexNode(Single, EvalNode* val, EvalNode* from) :
        val(val), type(SINGLE), from(from), to(nullptr) {}

    IndexNode(Left, EvalNode* val, EvalNode* from) :
        val(val), type(LEFT), from(from), to(nullptr) {}

    IndexNode(Right, EvalNode* val, EvalNode* to) :
        val(val), type(LEFT), from(nullptr), to(to) {}

    IndexNode(Double, EvalNode* val, EvalNode* from, EvalNode* to) :
        val(val), type(DOUBLE), from(from), to(to) {}

    std::string toString() override {
        std::string res;
        res += val->toString();
        res += "[";
        if (from) {
            res += from->toString();
        }
        if (type != SINGLE) {
            res += ":";
        }
        if (to) {
            res += to->toString();
        }
        res += "]";
        return res;
    }

private:
    enum Type { NONE, SINGLE, LEFT, RIGHT, DOUBLE};

    Type type;
    EvalNode* val;
    EvalNode* from;
    EvalNode* to;
};

struct InvokeNode : public EvalNode {
    InvokeNode(std::string name, std::vector<EvalNode*> args) : name(std::move(name)), args(std::move(args)) {}

    std::string toString() override {
        std::string res = name;

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
    std::string name;
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
    DictNode(std::unordered_map<std::string, EvalNode*> entries) : entries(std::move(entries)) {}

    std::string toString() override {
        std::string res = "{";
        if (!entries.empty()) {
            std::unordered_map<std::string, EvalNode*>::iterator it = entries.begin();
            ++it;
            while (it != entries.end()) {
                res += ", ";
                res += it->first;
                res += ": ";
                res += it->second->toString();
            }
        }
        res += "}";
        return res;
    }

private:
    std::unordered_map<std::string, EvalNode*> entries;
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
