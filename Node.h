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
            res += "-->";
            res += head->getString();
            res += "<--\n";
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

struct CommandNode : public Node {
    CommandNode(Node* name, Node* args) : name(name), args(args) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "command(";
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

struct FargsNode : public Node {
    FargsNode() : head(nullptr), tail(nullptr) {
        debugMessage();
    }

    FargsNode(Node* head, Node* tail) : head(head), tail(tail) {
        debugMessage();
    }

    std::string getString() override {
        if (!head) {
            return "args()";
        }

        std::string res;
        res += "args(";
        res += head->getString();

        if (tail) {
            res += ", ";
            res += tail->getString();
        }

        res += ")";
        return res;
    }

private:
    Node* head;
    Node* tail;
};

struct ListNode : public Node {
    ListNode(Node* args) : args(args) {
        debugMessage();
    }

    std::string getString() override {
        std::string res = "list(";
        res += args->getString();
        res += ")";
        return res;
    }

private:
    Node* args;
};

using QargsNode = FargsNode;
