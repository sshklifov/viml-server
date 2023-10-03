#pragma once

#include "BaseNode.hpp"

struct TryNode : public GroupNode {
    TryNode(const ExLexem& lexem) : GroupNode(lexem), finally(nullptr) {}

    int enumerate(EnumCallback cb) override {
        int cond = GroupNode::enumerate(cb);
        if (cond) {
            return cond & ENUM_STOP;
        }
        for (GroupNode* catchNode : catchNodes) {
            cond = catchNode->enumerate(cb);
            if (cond) {
                return cond & ENUM_STOP;
            }
        }
        if (finally) {
            finally->enumerate(cb);
            return cond & ENUM_STOP;
        }
        return ENUM_NEXT;
    }

    static const int id = CMD_try;

    Vector<GroupNode*> catchNodes;
    GroupNode* finally;
};

struct CatchNode : public GroupNode {
    CatchNode(const ExLexem& lexem) : GroupNode(lexem) {}

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;
        try {
            if (ends_excmd(*p)) {  // no argument, catch all errors
                pat = ".*";
                return p;
            } else {
                const char* q = skip_regexp_err(p + 1, *p);
                pat = FStr(p + 1, q);
                return q + 1;
            }
        } catch (msg& m) {
            rep.error(m);
            return nullptr;
        }
    }

    static const int id = CMD_catch;

private:
    FStr pat;
};

struct FinallyNode : public GroupNode {
    FinallyNode(const ExLexem& lexem) : GroupNode(lexem) {}

    static const int id = CMD_finally;
};

struct ThrowNode : public BaseNode {
    ThrowNode(const ExLexem& lexem) : BaseNode(lexem) { expr = nullptr; }

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;
        try {
            expr = eval1(p, f);
            return p;
        } catch (msg& m) {
            rep.error(m);
            return nullptr;
        }
    }

    static const int id = CMD_throw;

private:
    EvalExpr* expr;
};
