#pragma once

#include <ExCmdsEnum.hpp>
#include <ExLexem.hpp>

#include <Eval.hpp>
#include <EvalUtil.hpp>

#include <DiagnosticReporter.hpp>

#include <functional>

struct BaseNode {
    using EnumCallback = std::function<void(BaseNode*)>;

    enum {PARSE_STOP, PARSE_NEXT};

    BaseNode(const ExLexem& lexem) : lexem(lexem) {}
    virtual ~BaseNode() {}

    virtual void enumerate(EnumCallback cb) {
        cb(this);
    }

    virtual int getId() { return lexem.cmdidx; }

    virtual bool parse(DiagnosticReporter& rep) {
        assert(false); // TODO make pure
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

    ExLexem lexem;
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

struct RootNode : GroupNode {
    RootNode() : GroupNode(ExLexem()) {}

    int getId() override {
        assert(false);
        return CMD_SIZE;
    }

    bool parse(DiagnosticReporter& rep) override {
        assert(false);
        return PARSE_STOP;
    }
};

struct ExNode : BaseNode {
    ExNode(const ExLexem& lexem) : BaseNode(lexem) {}
};

struct IfNode : public GroupNode {
    IfNode(const ExLexem& lexem) : GroupNode(lexem), cond(nullptr), elseIfNode(nullptr) {}

    void enumerate(EnumCallback cb) override {
        GroupNode::enumerate(cb);
        if (elseIfNode) {
            elseIfNode->enumerate(cb);
        }
    }

    bool parse(DiagnosticReporter& rep) override {
        // const char* p = lexem.qargs.begin;
        // BoundReporter boundRep(rep, lexem);
        // cond = check_and_eval(p, lexem.qargs.length(), boundRep, f);
        // TODO how is the hierarchy?
        // ex_if?
        return PARSE_STOP;
    }

    static const int id = CMD_if;

    EvalFactory f;
    EvalExpr* cond;

    GroupNode* elseIfNode;
};

struct ElseIfNode : public IfNode {
    ElseIfNode(const ExLexem& lexem) : IfNode(lexem) {}

    static const int id = CMD_elseif;
};

struct ElseNode : public GroupNode {
    ElseNode(const ExLexem& lexem) : GroupNode(lexem) {}

    static const int id = CMD_else;
};

struct WhileNode : public GroupNode {
    WhileNode(const ExLexem& lexem) : GroupNode(lexem) {}

    static const int id = CMD_while;
};

struct ForNode : public GroupNode {
    ForNode(const ExLexem& lexem) : GroupNode(lexem) {}

    static const int id = CMD_for;
};

struct FunctionNode : public GroupNode {
    FunctionNode(const ExLexem& lexem) : GroupNode(lexem) {}

    static const int id = CMD_function;
};

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

    static const int id = CMD_catch;
};

struct FinallyNode : public GroupNode {
    FinallyNode(const ExLexem& lexem) : GroupNode(lexem) {}

    static const int id = CMD_finally;
};
