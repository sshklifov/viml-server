#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include <Constants.hpp>
#include <StringView.hpp>

struct Block {
    Block() {}

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

    virtual void addChild(Block* child) {
        body.push_back(child);
    }

protected:
    std::vector<Block*> body;
};

struct RootBlock : Block {
    virtual int getId() {
        return ROOT;
    }
};

struct ExBlock : Block {
    ExBlock(int id, const StringView& name, const StringView& qargs) :
        id(id), name(name.begin, name.end), qargs(qargs.begin, qargs.end) {}

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "Ex(" + name;
        if (!qargs.empty()) {
            res += (", " + qargs);
        }
        res += ")\n";
        return res;
    }

    void addChild(Block* child) override {
        throw std::runtime_error("Cannot add child to leaf node"); // TODO
    }

    int id;
    std::string name;
    std::string qargs;
};

struct IfBlock : public Block {
    IfBlock(const StringView& qargs) : expr1(qargs.begin, qargs.end), elseBlock(nullptr) {}

    static const int id = IF;

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "If(" + expr1 + ")\n";
        res += bodyToString();

        if (elseBlock) {
            res += "Else()\n";
            res += elseBlock->toString();
        }
        res += "Endif()\n";
        return res;
    }

    std::string expr1;
    Block* elseBlock;
};

struct ElseBlock : public Block {
    static const int id = ELSE;

    int getId() override {
        return id;
    };
};

struct WhileBlock : public Block {
    WhileBlock(const StringView& qargs) : expr1(qargs.begin, qargs.end) {}

    static const int id = WHILE;

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "While(" + expr1 + ")\n";
        res += bodyToString();
        res += "Endwhile()\n";
        return res;
    }

    std::string expr1;
};

struct ForBlock : public Block {
    ForBlock(const StringView& qargs) : expr1(qargs.begin, qargs.end) {}

    static const int id = FOR;

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "For(" + expr1 + ")\n";
        res += bodyToString();
        res += "Endfor()\n";
        return res;
    }

    std::string expr1;
};

struct FunctionBlock : public Block {
    FunctionBlock(const StringView& qargs) : expr1(qargs.begin, qargs.end) {}

    static const int id = FUNCTION;

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "Function(" + expr1 + ")\n";
        res += bodyToString();
        res += "Endfunction()\n";
        return res;
    }

    std::string expr1;
};

struct TryBlock : public Block {
    TryBlock(const StringView& qargs) : expr1(qargs.begin, qargs.end) {}

    static const int id = TRY;

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "Try(" + expr1 + ")\n";
        res += bodyToString();
        res += "EndTry()\n";
        return res;
    }

    std::string expr1;
    std::vector<Block*> catchBlocks;
};

struct CatchBlock : public Block {
    CatchBlock(const StringView& pattern) : pattern(pattern.begin, pattern.end) {}

    static const int id = CATCH;

    int getId() override {
        return id;
    }

    std::string toString() override {
        std::string res = "Catch(" + pattern + ")\n";
        res += bodyToString();
        return res;
    }

    std::string pattern;
};
