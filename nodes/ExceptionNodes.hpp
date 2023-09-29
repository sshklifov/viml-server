#pragma once

#include "NodeDefs.hpp"
#include "DoCmdUtil.hpp"

struct TryNode : public GroupNode {
    TryNode(const ExLexem& lexem) : GroupNode(lexem), finally(nullptr) {}

    void enumerate(EnumCallback cb) override {
        GroupNode::enumerate(cb);
        for (GroupNode* catchNode : catchNodes) {
            catchNode->enumerate(cb);
        }
        if (finally) {
            finally->enumerate(cb);
        }
    }

    static const int id = CMD_try;

    Vector<GroupNode*> catchNodes;
    GroupNode* finally;
};

struct CatchNode : public GroupNode {
    CatchNode(const ExLexem& lexem) : GroupNode(lexem) {}

    const char* parseInternal(BoundReporter& rep) override {
        const char* p = lex.qargs;
        try {
            if (ends_excmd(*p)) {  // no argument, catch all errors
                pat = ".*";
                return p;
            } else {
                const char* patBegin = p + 1;
                p = skip_regexp_err(patBegin + 1, *patBegin);
                pat = FStr(patBegin, p + 1);
                return p + 1;
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
