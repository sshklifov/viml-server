#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include <ExLexer.hpp>
#include <Constants.hpp>

struct Block {
    Block(const ExLexem& lexem) : lexem(lexem) {}

    virtual ~Block() {}

    virtual int getId() = 0;

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
    ExBlock(const ExLexem& lexem, int id) : Block(lexem), id(id) {}

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "Ex(" + lexem.name.toString();
        if (!lexem.qargs.empty()) {
            res += (", " + lexem.qargs.toString());
        }
        res += ")\n";
        return res;
    }

    int id;
};

struct IfBlock : public Block {
    IfBlock(const ExLexem& lexem) : Block(lexem), elseBlock(nullptr) {}

    static const int id = IF;

    int getId() override {
        return id;
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

    Block* elseBlock;
};

struct ElseBlock : public Block {
    ElseBlock(const ExLexem& lexem) : Block(lexem) {}

    static const int id = ELSE;

    int getId() override {
        return id;
    };

    std::string toString() override {
        assert(false);
        return "";
    }
};

struct WhileBlock : public Block {
    WhileBlock(const ExLexem& lexem) : Block(lexem) {}

    static const int id = WHILE;

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "While(" + lexem.qargs.toString() + ")\n";
        res += bodyToString();
        res += "Endwhile()\n";
        return res;
    }
};

struct ForBlock : public Block {
    ForBlock(const ExLexem& lexem) : Block(lexem) {}

    static const int id = FOR;

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "For(" + lexem.qargs.toString() + ")\n";
        res += bodyToString();
        res += "Endfor()\n";
        return res;
    }
};

struct FunctionBlock : public Block {
    FunctionBlock(const ExLexem& lexem) : Block(lexem) {}

    static const int id = FUNCTION;

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "Function(" + lexem.qargs.toString() + ")\n";
        res += bodyToString();
        res += "Endfunction()\n";
        return res;
    }

    std::string expr1;
};

struct TryBlock : public Block {
    TryBlock(const ExLexem& lexem) : Block(lexem), finally(nullptr) {}

    static const int id = TRY;

    int getId() override {
        return id;
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

    std::vector<Block*> catchBlocks;
    Block* finally;
};

struct CatchBlock : public Block {
    CatchBlock(const ExLexem& lexem) : Block(lexem) {}

    static const int id = CATCH;

    int getId() override {
        return id;
    }

    std::string toString() override {
        assert(false);
        return "";
    }
};

struct FinallyBlock : public Block {
    FinallyBlock(const ExLexem& lexem) : Block(lexem) {}

    static const int id = FINALLY;

    int getId() override {
        return id;
    }

    std::string toString() override {
        assert(false);
        return "";
    }
};
