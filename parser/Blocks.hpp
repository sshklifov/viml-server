#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include <ExLexer.hpp>
#include <ExConstants.hpp>

#include <Eval.hpp>

struct Block {
    Block(const ExLexem& lexem) : lexem(lexem), evalCmd(nullptr) {}

    virtual ~Block() {}

    void parse(EvalFactory& factory) {
        if (evalParseSupported(lexem)) {
            evalCmd = evalParse(lexem, factory);
        }
    }

    virtual void parseRecursive(EvalFactory& factory) {
        parse(factory);
        for (Block* block : body) {
            block->parseRecursive(factory);
        }
    }

    virtual int getId() { return lexem.exDictIdx; }

    virtual std::string toString() { return bodyToString(); }

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

    EvalCommand* evalCmd;
    ExLexem lexem;
    std::vector<Block*> body;
};

struct RootBlock : Block {
    RootBlock() : Block(ExLexem()) {}

    static const int id = ROOT;

    virtual int getId() {
        return id;
    }
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

    void parseRecursive(EvalFactory& factory) override {
        Block::parseRecursive(factory);
        if (elseBlock) {
            elseBlock->parseRecursive(factory);
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

    virtual void parseRecursive(EvalFactory& factory) override {
        Block::parseRecursive(factory);
        for (Block* catchBlock : catchBlocks) {
            catchBlock->parseRecursive(factory);
        }
        if (finally) {
            finally->parseRecursive(factory);
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
