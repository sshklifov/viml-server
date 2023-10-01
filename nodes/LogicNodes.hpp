#pragma once

#include "BaseNode.hpp"

struct IfNode : public GroupNode {
    IfNode(const ExLexem& lexem) : GroupNode(lexem), cond(nullptr), elseIfNode(nullptr) {}

    void enumerate(EnumCallback cb) override {
        GroupNode::enumerate(cb);
        if (elseIfNode) {
            elseIfNode->enumerate(cb);
        }
    }

    const char* parseArgs(BoundReporter& rep) override {
        try {
            const char* p = lex.qargs;
            cond = eval1(p, f);
            return p;
        } catch (msg& m) {
            rep.error(m);
            return nullptr;
        }
    }

    static const int id = CMD_if;

    GroupNode* elseIfNode;
private:
    EvalExpr* cond;
};

struct ElseIfNode : public IfNode {
    ElseIfNode(const ExLexem& lexem) : IfNode(lexem) {}

    static const int id = CMD_elseif;
};

struct ElseNode : public GroupNode {
    ElseNode(const ExLexem& lexem) : GroupNode(lexem) {}

    static const int id = CMD_else;
};
