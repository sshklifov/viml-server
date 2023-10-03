#pragma once

#include "BaseNode.hpp"

struct IfNode : public GroupNode {
    IfNode(const ExLexem& lexem) : GroupNode(lexem), elseIfNode(nullptr), cond(nullptr) {}

    int enumerate(EnumCallback cb) override {
        int cond = GroupNode::enumerate(cb);
        if (cond) {
            return cond & ENUM_STOP;
        }
        if (elseIfNode) {
            cond = elseIfNode->enumerate(cb);
            return cond & ENUM_STOP;
        }
        return ENUM_NEXT;
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
