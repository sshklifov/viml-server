#pragma once

#include <ExLexem.hpp>
#include <DiagnosticReporter.hpp>

#include <DoCmd.hpp>
#include <Eval.hpp>

#include <StringMap.hpp>
#include <functional>

struct Variable {
    Variable(int global = 0, int locked = 0, VarType type = VAR_UNKNOWN) :
        global(global), locked(locked), type(type) {}

    int global;
    int locked;
    VarType type;
};

struct UserFunction {
    UserFunction(int nargs = 0, int varargs = 0, int flags = 0) :
        nargs(nargs), varargs(varargs), flags(flags) {}

    int nargs;
    int varargs;
    int flags;
};

// TODO user commands

struct EvalContext {
    emhash8::StringMap<Variable> vars;
    emhash8::StringMap<UserFunction> funs;
};

struct BaseNode {
    enum EnumCond {ENUM_NEXT = 0, ENUM_PRUNE = 1, ENUM_STOP = 2};
    using EnumCallback = std::function<int(BaseNode*)>;

    BaseNode(const ExLexem& lexem) : lex(lexem) {}
    virtual ~BaseNode() {}

    virtual int enumerate(EnumCallback cb) {
        return cb(this);
    }

    virtual int getId() { return lex.cmdidx; }

    virtual void parse(DiagnosticReporter& rep, const char*& nextcmd) {
        BoundReporter boundRep(rep, lex);
        const char* p = parseArgs(boundRep);
        hasParseErrors = (p == nullptr);
        if (!hasParseErrors) {
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

    virtual void eval(DiagnosticReporter& rep, EvalContext& ctx) {
        // TODO
    }
    
    virtual Range range() {
        if (!hasParseErrors) {
            // Use a smaller range in case of '|' separation
            return lex.locator.resolve(lex.name, lex.nextcmd);
        } else {
            // Fallback to full range, no commands after '|' separator
            return lex.locator.resolve();
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
            return nullptr;
        }
    }

    ExLexem lex;
    EvalFactory f;
    int hasParseErrors;

protected:
    virtual const char* parseArgs(BoundReporter&) { return lex.qargs; }
};

struct GroupNode : public BaseNode {
    GroupNode(const ExLexem& lexem) : BaseNode(lexem) {}

    int enumerate(EnumCallback cb) override {
        int cond = cb(this);
        if (cond) {
            return cond & ENUM_STOP;
        }
        for (BaseNode* node : children) {
            cond = node->enumerate(cb);
            if (cond) {
                return cond & ENUM_STOP;
            }
        }
        return ENUM_NEXT;
    }

    Range bbox() override {
        Range res = range();
        if (!children.empty()) {
            res.join(children.last()->bbox());
        }
        return res;
    }

    BaseNode* findNode(const Position& p) override {
        if (range().has(p)) {
            return this;
        }

        int lo = 0;
        int hi = children.count();
        while (hi - lo > 1) {
            int mid = (lo + hi) / 2;
            int loc = children[mid]->range().locate(p);
            if (loc < 0) {
                hi = mid;
            } else {
                lo = mid;
            }
        }
        if (!children.empty()) {
            return children[lo]->findNode(p);
        } else {
            return nullptr;
        }
    }

    Vector<BaseNode*> children;
};
