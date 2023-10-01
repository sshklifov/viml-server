#pragma once

#include <ExLexem.hpp>
#include <ExCmdsDefs.hpp>
#include <DoCmdUtil.hpp>
#include <Eval.hpp>

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

    virtual void parse(DiagnosticReporter& rep, const char*& nextcmd) {
        BoundReporter boundRep(rep, lex);
        const char* p = parseArgs(boundRep);
        parseErrors = (p == nullptr);
        if (!parseErrors) {
            // Check for trailing bar
            lex.nextcmd = p;
            if (ends_excmd(*p)) {
                if (*p == '|') {
                    nextcmd = p + 1;
                }
            } else {
                boundRep.error("Trailing characters", p);
            }
        }
    }
    
    virtual Range range() {
        if (parseErrors) {
            return lex.locator.resolve();
        } else {
            int start = lex.name - lex.cmdline;
            int end = lex.nextcmd - lex.cmdline;
            return lex.locator.resolve(start, end);
        }
    }

    virtual Range bbox() {
        return range();
    }

    virtual BaseNode* findNode(const Position& p) {
        if (range().has(p)) {
            return this;
        } else {
            return nullptr;
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
    EvalFactory f;
    int parseErrors;

protected:
    virtual const char* parseArgs(BoundReporter& rep) { return lex.qargs; }
};

struct GroupNode : public BaseNode {
    GroupNode(const ExLexem& lexem) : BaseNode(lexem) {}

    void enumerate(EnumCallback cb) override {
        cb(this);
        for (BaseNode* node : body) {
            node->enumerate(cb);
        }
    }

    Range bbox() override {
        Range res = range();
        if (!body.empty()) {
            res.join(body.last()->bbox());
        }
        return res;
    }

    BaseNode* findNode(const Position& p) override {
        if (range().has(p)) {
            return this;
        }

        int lo = 0;
        int hi = body.count();
        while (hi - lo > 1) {
            int mid = (lo + hi) / 2;
            int loc = body[mid]->range().locate(p);
            if (loc < 0) {
                hi = mid;
            } else {
                lo = mid;
            }
        }
        if (!body.empty()) {
            return body[lo]->findNode(p);
        } else {
            return nullptr;
        }
    }

    Vector<BaseNode*> body;
};
