#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <functional>

#include <ExLexer.hpp>
#include <ExConstants.hpp>

#include <EvalCommand.hpp>

struct Block {
    // TODO needed? enum EnumResult {STOP, CONTINUE};
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

    virtual std::string toString() = 0;

    std::string bodyToString() {
        std::string result;
        for (Block* child : body) {
            result += child->toString();
        }
        return result;
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

    std::string toString() override { return bodyToString(); }
};

struct ExBlock : Block {
    ExBlock(const ExLexem& lexem) : Block(lexem) {}

    std::string toString() override {
        std::string res = "Ex(" + lexem.name.toString();
        if (!lexem.qargs.empty()) {
            res += (", " + lexem.qargs.toString());
        }
        res += ")\n";
        return res;
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

    std::string toString() override {
        std::string res = "If(" + lexem.qargs.toString() + ")\n";
        res += bodyToString();

        if (elseBlock) {
            res += "Else()\n";
            res += elseBlock->bodyToString();
        }
        res += "Endif()\n";
        return res;
    }

    static const int id = IF;
    Block* elseBlock;
};

struct ElseBlock : public Block {
    ElseBlock(const ExLexem& lexem) : Block(lexem) {}

    std::string toString() override {
        assert(false);
        return "";
    }

    static const int id = ELSE;
};

struct WhileBlock : public Block {
    WhileBlock(const ExLexem& lexem) : Block(lexem) {}

    std::string toString() override {
        std::string res = "While(" + lexem.qargs.toString() + ")\n";
        res += bodyToString();
        res += "Endwhile()\n";
        return res;
    }

    static const int id = WHILE;
};

struct ForBlock : public Block {
    ForBlock(const ExLexem& lexem) : Block(lexem) {}

    std::string toString() override {
        std::string res = "For(" + lexem.qargs.toString() + ")\n";
        res += bodyToString();
        res += "Endfor()\n";
        return res;
    }

    static const int id = FOR;
};

struct FunctionBlock : public Block {
    FunctionBlock(const ExLexem& lexem) : Block(lexem) {}

    std::string toString() override {
        std::string res = "Function(" + lexem.qargs.toString() + ")\n";
        res += bodyToString();
        res += "Endfunction()\n";
        return res;
    }

    static const int id = FUNCTION;
    std::string expr1;
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

    std::string toString() override {
        std::string res = "Try(" + lexem.qargs.toString() + ")\n";
        res += bodyToString();

        for (Block* block : catchBlocks) {
            res += ("Catch(" + block->lexem.qargs.toString() + ")\n");
            res += block->bodyToString();
        }

        if (finally) {
            res += "Finally()\n";
            res += finally->bodyToString();
        }

        res += "EndTry()\n";
        return res;
    }

    static const int id = TRY;
    std::vector<Block*> catchBlocks;
    Block* finally;
};

struct CatchBlock : public Block {
    CatchBlock(const ExLexem& lexem) : Block(lexem) {}

    std::string toString() override {
        assert(false);
        return "";
    }

    static const int id = CATCH;
};

struct FinallyBlock : public Block {
    FinallyBlock(const ExLexem& lexem) : Block(lexem) {}

    std::string toString() override {
        assert(false);
        return "";
    }

    static const int id = FINALLY;
};
