#pragma once

#include <string>

struct Node {
    virtual ~Node() {}
    virtual std::string getString() = 0;

    void debugMessage() {
        const int debugEnabled = 0;
        if (debugEnabled) {
            std::string s = getString();
            printf("Constructing Node: %s\n", s.c_str());
        }
    }
};

struct GenericListNode : public Node {
    GenericListNode(const char* name, Node* head, Node* tail) : name(name), head(head), tail(tail) {}

    std::string getString() override {
        std::string res;
        res += name;
        res += "(";

        if (head) {
            res += head->getString();
            if (tail) {
                res += ", ";
                res += tail->getString();
            }
        }

        res += ")";
        return res;
    }

private:
    std::string name;
    Node* head;
    Node* tail;
};

struct LexemNode : public Node {
    LexemNode(const std::string& name) : name(name) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "lex(";
        res += name;
        res += ")";
        return res;
    }

private:
    std::string name;
};

struct LineNode : public Node {
    LineNode(Node* head = nullptr, Node* tail = nullptr) : head(head), tail(tail) {
        debugMessage();
    }

    std::string getString() override {

        std::string res;
        if (head) {
            res += head->getString();
            res += "\n";
        } else {
            res += "<empty-line>\n";
        }

        if (tail) {
            res += tail->getString();
        }

        return res;
    }

private:
    Node* head;
    Node* tail;
};

struct FunCallNode : public Node {
    FunCallNode(Node* name, Node* args) : name(name), args(args) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "fcall(";
        res += name->getString();
        res += ", ";

        if (args) {
            res += args->getString();
        } else {
            res += "<no-args>";
        }

        res += ")";
        return res;
    }

private:
    Node* name;
    Node* args;
};

struct LambdaNode : public Node {
    LambdaNode(Node* args, Node* body) : args(args), body(body) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "lambda(";
        res += args->getString();
        res += ", ";
        res += body->getString();
        res += ")";
        return res;
    }

private:
    Node* args;
    Node* body;
};

struct InfixOpNode : public Node {
    InfixOpNode(Node* lhs, Node* rhs, const char* op) : lhs(lhs), rhs(rhs), op(op) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "op(";
        res += "'";
        res += op;
        res += "', ";

        res += lhs->getString();
        res += ", ";
        res += rhs->getString();
        res += ")";
        return res;
    }

private:
    Node* lhs;
    Node* rhs;
    std::string op;
};

struct PrefixOpNode : public Node {
    PrefixOpNode(Node* node, const char* op) : node(node), op(op) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "prefix(";
        res += "'";
        res += op;
        res += "', ";

        res += node->getString();
        res += ")";

        return res;
    }

private:
    Node* node;
    std::string op;
};

struct ExNode : public Node {
    ExNode(Node* name, Node* args) : name(name), args(args) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "ex(";
        res += name->getString();
        res += ", ";

        if (args) {
            res += args->getString();
        } else {
            res += "<no-args>";
        }

        res += ")";
        return res;
    }

private:
    Node* name;
    Node* args;
};

struct ListNode : public Node {
    ListNode(Node* args) : args(args) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "list(";
        if (args) {
            res += args->getString();
        } else {
            res += "<empty>";
        }
        res += ")";
        return res;
    }

private:
    Node* args;
};

struct DictNode : public Node {
    DictNode(Node* args) : args(args) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "dict(";
        if (args) {
            res += args->getString();
        }
        res += ")";
        return res;
    }

private:
    Node* args;
};

struct IfBlockNode : public Node {
    IfBlockNode(Node* cond, Node* body, Node* el = nullptr) : cond(cond), body(body), el(el) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "if ";
        res += cond->getString();
        res += "\n";
        if (body) {
            res += body->getString();
        } else {
            res += "<empty-body>\n";
        }
        if (el) {
            res += "else\n";
            res += el->getString();
        }
        res += "endif";
        return res;
    }

private:
    Node* cond;
    Node* body;
    Node* el;
};

struct WhileBlockNode : public Node {
    WhileBlockNode(Node* cond, Node* body) : cond(cond), body(body) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "while ";
        res += cond->getString();
        res += "\n";
        if (body) {
            res += body->getString();
        } else {
            res += "<empty-body>\n";
        }
        res += "endwhile";
        return res;
    }

private:
    Node* cond;
    Node* body;
};

struct ForBlockNode : public Node {
    ForBlockNode(Node* var, Node* iterable, Node* body) : var(var), iterable(iterable), body(body) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "for ";
        res += var->getString();
        res += " in ";
        res += iterable->getString();
        res += "\n";
        if (body) {
            res += body->getString();
        } else {
            res += "<empty-body>\n";
        }
        res += "endfor";
        return res;
    }

private:
    Node* var;
    Node* iterable;
    Node* body;
};

struct TernaryNode : public Node {
    TernaryNode(Node* cond, Node* left, Node* right) : cond(cond), left(left), right(right) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "ter(";
        res += cond->getString();
        res += ", ";
        res += left->getString();
        res += ", ";
        res += right->getString();
        res += ")";
        return res;
    }

private:
    Node* cond;
    Node* left;
    Node* right;
};

struct FunctionBlockNode : public Node {
    FunctionBlockNode(Node* name, Node* args, Node* body) : name(name), args(args), body(body) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "function ";
        res += name->getString();
        res += " ";
        if (args) {
            res += args->getString();
        } else {
            res += "<empty-args>";
        }
        res += "\n";

        if (body) {
            res += body->getString();
        } else {
            res += "<empty-body>\n";
        }
        res += "endfunction";
        return res;
    }

private:
    Node* name;
    Node* args;
    Node* body;
};

struct LetNode : public Node {
    LetNode(Node* name, Node* value, const char* op) : name(name), value(value), op(op) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "let ";
        res += name->getString();
        res += " ";
        res += op;
        res += " ";
        res += value->getString();

        return res;
    }

private:
    const char* op;
    Node* name;
    Node* value;
};

struct CommandNode : public Node {
    CommandNode(Node* name, Node* attrs, Node* body) : name(name), attrs(attrs), body(body) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "command(";
        res += name->getString();
        if (attrs) {
            res += ", ";
            res += attrs->getString();
        }

        if (body) {
            res += ", ";
            res += body->getString();
        }
        res += ")";
        return res;
    }

private:
    Node* name;
    Node* attrs;
    Node* body;
};

struct IndexNode : public Node {
    IndexNode(Node* val, Node* idx, Node* to = nullptr) : val(val), idx(idx), to(to) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "index(";
        res += val->getString();
        res += ", ";
        res += idx->getString();
        if (to) {
            res += ", ";
            res += to->getString();
        }
        res += ")";
        return res;
    }

private:
    Node* val;
    Node* idx;
    Node* to;
};

struct KeyValueNode : public Node {
    KeyValueNode(Node* key, Node* val) : key(key), val(val) {
        debugMessage();
    }

    std::string getString() override {
        std::string res;
        res += key->getString();
        res += "=";
        res += val->getString();
        return res;
    }

private:
    Node* key;
    Node* val;
};

struct OptionNode : public Node {
    OptionNode(Node* name) : name(name) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "opt(";
        res += name->getString();
        res += ")";
        return res;
    }

private:
    Node* name;
};

struct RegNode : public Node {
    RegNode(Node* name) : name(name) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "reg(";
        res += name->getString();
        res += ")";
        return res;
    }

private:
    Node* name;
};

struct FargsNode : public GenericListNode {
    FargsNode(Node* head = nullptr, Node* tail = nullptr) : GenericListNode("args", head, tail) {
        debugMessage();
    }
};

struct QargsNode : public GenericListNode {
    QargsNode(Node* head = nullptr, Node* tail = nullptr) : GenericListNode("qargs", head, tail) {
        debugMessage();
    }
};

struct ParamsNode : public GenericListNode {
    ParamsNode(Node* head, Node* tail) : GenericListNode("params", head, tail) {
        debugMessage();
    }
};

struct AttrsNode : public GenericListNode {
    AttrsNode(Node* head, Node* tail) : GenericListNode("attrs", head, tail) {
        debugMessage();
    }
};
