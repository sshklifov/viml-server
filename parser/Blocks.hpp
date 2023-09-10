#pragma once

#include <FStr.hpp>

#include <vector>
#include <functional>

#include <ExLexer.hpp>
#include <ExConstants.hpp>
#include <EvalCommand.hpp>

struct Block {
    using EnumCallback = std::function<void(Block*)>;

    Block(const ExLexem& lexem) : lexem(lexem), evalCmd(nullptr) {}
    virtual ~Block() {}

    virtual void enumerate(EnumCallback cb) {
        cb(this);
        for (Block* block : body) {
            cb(block);
        }
    }

    virtual int getId() { return lexem.exDictIdx; }

    virtual void appendStr(FStr& res) = 0;

    void appendBodyStr(FStr& res) {
        for (Block* child : body) {
            child->appendStr(res);
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

    void addToScope(Block* child) {
        body.push_back(child);
    }

    ExLexem lexem;
    EvalCommand* evalCmd;

    std::vector<Block*> body;
};

struct RootBlock : Block {
    RootBlock() : Block(ExLexem()) {}

    int getId() override {
        return -1;
    }

    void appendStr(FStr& s) override { appendBodyStr(s); }
};

struct ExBlock : Block {
    ExBlock(const ExLexem& lexem) : Block(lexem) {}

    void appendStr(FStr& res) override {
        res.appendf("Ex({}", lexem.name);
        if (!lexem.qargs.empty()) {
            res.appendf(", {}", lexem.qargs);
        }
        res.append(")\n");
    }
};

struct IfBlock : public Block {
    IfBlock(const ExLexem& lexem) : Block(lexem), elseBlock(nullptr) {}

    void enumerate(EnumCallback cb) override {
        Block::enumerate(cb);
        if (elseBlock) {
            elseBlock->enumerate(cb);
        }
    }

    void appendStr(FStr& res) override {
        res.appendf("If({})\n", lexem.qargs);
        appendBodyStr(res);
        if (elseBlock) {
            elseBlock->appendStr(res);
        }
        res.append("Endif()\n");
    }

    static const int id = IF;
    Block* elseBlock;
};

struct ElseBlock : public Block {
    ElseBlock(const ExLexem& lexem) : Block(lexem) {}

    void appendStr(FStr& res) override {
        res.append("Else()\n");
        appendBodyStr(res);
    }

    static const int id = ELSE;
};

struct WhileBlock : public Block {
    WhileBlock(const ExLexem& lexem) : Block(lexem) {}

    void appendStr(FStr& res) override {
        res.appendf("While({})\n", lexem.qargs);
        appendBodyStr(res);
        res.append("Endwhile()\n");
    }

    static const int id = WHILE;
};

struct ForBlock : public Block {
    ForBlock(const ExLexem& lexem) : Block(lexem) {}

    void appendStr(FStr& res) override {
        res.appendf("For({})\n", lexem.qargs);
        appendBodyStr(res);
        res.append("Endfor()\n");
    }

    static const int id = FOR;
};

struct FunctionBlock : public Block {
    FunctionBlock(const ExLexem& lexem) : Block(lexem) {}

    void appendStr(FStr& res) override {
        res.appendf("Function({})\n", lexem.qargs);
        appendBodyStr(res);
        res.append("Endfunction()\n");
    }

    static const int id = FUNCTION;
};

struct TryBlock : public Block {
    TryBlock(const ExLexem& lexem) : Block(lexem), finally(nullptr) {}

    void enumerate(EnumCallback cb) override {
        Block::enumerate(cb);
        for (Block* catchBlock : catchBlocks) {
            catchBlock->enumerate(cb);
        }
        if (finally) {
            finally->enumerate(cb);
        }
    }

    void appendStr(FStr& res) override {
        res.appendf("Try({})\n", lexem.qargs);
        appendBodyStr(res);

        for (Block* block : catchBlocks) {
            block->appendStr(res);
        }
        if (finally) {
            finally->appendStr(res);
        }

        res.append("EndTry()\n");
    }

    static const int id = TRY;
    std::vector<Block*> catchBlocks;
    Block* finally;
};

struct CatchBlock : public Block {
    CatchBlock(const ExLexem& lexem) : Block(lexem) {}

    void appendStr(FStr& res) override {
        res.appendf("Catch({})\n", lexem.qargs);
        appendBodyStr(res);
    }

    static const int id = CATCH;
};

struct FinallyBlock : public Block {
    FinallyBlock(const ExLexem& lexem) : Block(lexem) {}

    void appendStr(FStr& res) override {
        res.appendf("Finally()\n");
        appendBodyStr(res);
    }

    static const int id = FINALLY;
};
