#pragma once

#include "BaseNode.hpp"

struct EchoNode : public BaseNode {
    EchoNode(const ExLexem& lexem) : BaseNode(lexem) {}

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;
        try {
            while (!ends_notrlcom(*p)) {
                exprs.emplace(eval1(p, f));
            }
            return p;
        } catch (msg& m) {
            rep.error(m);
            f.clear();
            return nullptr;
        }
    }

    static const int id = CMD_echo;

private:
    Vector<EvalExpr*> exprs;
};
