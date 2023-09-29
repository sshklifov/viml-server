#pragma once

#include <ExLexem.hpp>
#include <ExCmdsDefs.hpp>
#include <DoCmdUtil.hpp>

#include <DiagnosticReporter.hpp>

#include <functional>

struct BaseNode {
    using EnumCallback = std::function<void(BaseNode*)>;

    BaseNode(const ExLexem& lexem) : lex(lexem) {}
    virtual ~BaseNode() {}

    virtual void enumerate(EnumCallback cb) {
        cb(this);
    }

    virtual int getId() { return lex.cmdidx; }

    virtual void parse(DiagnosticReporter& rep) {
        BoundReporter boundRep(rep, lex);
        const char* p = parseInternal(boundRep);
        if (!p) {
            return;
        }
        // Check for trailing bar
        int cmd_argt = cmdnames[lex.cmdidx].cmd_argt;
        if (!(cmd_argt & EX_TRLBAR)) {
            if (ends_excmd(*p)) {
                if (*p == '|') {
                    lex.nextcmd = p + 1;
                }
            } else {
                boundRep.error("Trailing characters", p);
            }
        }
    }

    template <typename T>
    T* cast() {
        if (T::id == getId()) {
            return static_cast<T*>(this);
        } else {
            assert(false);
            return nullptr;
        }
    }

    ExLexem lex;

protected:
    virtual const char* parseInternal(BoundReporter& rep) {
        return lex.qargs;
    }
};

struct GroupNode : public BaseNode {
    GroupNode(const ExLexem& lexem) : BaseNode(lexem) {}

    void enumerate(EnumCallback cb) override {
        cb(this);
        for (BaseNode* node : body) {
            node->enumerate(cb);
        }
    }

    Vector<BaseNode*> body;
};
