#pragma once

/* #include <ExLexer.hpp> */
/* #include <ExConstants.hpp> */
/* #include <EvalCommand.hpp> */

#include <ExLexem.hpp>
#include <ExCmdsEnum.hpp>

#include <functional>

struct BaseExpr {
    using EnumCallback = std::function<void(BaseExpr*)>;

    BaseExpr(const ExLexem& lexem) : lexem(lexem) {}
    virtual ~BaseExpr() {}

    virtual void enumerate(EnumCallback cb) {
        cb(this);
    }

    virtual int getId() { return lexem.cmdidx; }

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

struct GroupNode : public BaseExpr {
    GroupNode(const ExLexem& lexem) : BaseExpr(lexem) {}

    void enumerate(EnumCallback cb) override {
        cb(this);
        for (BaseExpr* node : body) {
            node->enumerate(cb);
        }
    }

    Vector<BaseExpr*> body;
};

struct RootNode : GroupNode {
    RootNode() : GroupNode(ExLexem()) {}

    int getId() override {
        assert(false);
        return CMD_SIZE;
    }
};

struct ExNode : BaseExpr {
    ExNode(const ExLexem& lexem) : BaseExpr(lexem) {}
};

struct IfNode : public GroupNode {
    IfNode(const ExLexem& lexem) : GroupNode(lexem), elseIfNode(nullptr) {}

    void enumerate(EnumCallback cb) override {
        GroupNode::enumerate(cb);
        if (elseIfNode) {
            elseIfNode->enumerate(cb);
        }
    }

    static const int id = CMD_if;

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
