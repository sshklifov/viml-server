#pragma once

#include <string>
#include <format>
#include <vector>

struct Block {
    virtual std::string toString() = 0;
};

struct ExBlock : Block {
    ExBlock(const std::string& name, const std::string& body = "") : name(name), body(body) {}

    std::string toString() override {
        std::string res = std::format("Ex({}", name);
        if (!body.empty()) {
            res += std::format(", {}", body);
        }
        res += ")\n";
        return res;
    }

private:
    std::string name;
    std::string body;
};

struct TryBlock : public Block {
    TryBlock(Block* body, Block* finally = nullptr) : body(body), finally(finally) {}

    std::string toString() override {
        std::string res = "Try()\n";
        res += body->toString();
        if (finally) {
            res += "Finally()\n";
            res += body->toString();
        }
        res += "Endtry()\n";
        return res;
    }

private:
    Block* body;
    Block* finally;
};

struct WhileBlock : public Block {
    WhileBlock(const std::string& expr1, Block* body) : expr1(expr1), body(body) {}

    std::string toString() override {
        std::string res = std::format("While({})\n", expr1);
        res += body->toString();
        res += "Endwhile()\n";
        return res;
    }

private:
    std::string expr1;
    Block* body;
};

struct ForBlock : public Block {
    ForBlock(const std::string& args, Block* body) : args(args), body(body) {}

    std::string toString() override {
        std::string res = std::format("For({})\n", args);
        res += body->toString();
        res += "Endfor()\n";
        return res;
    }

private:
    std::string args;
    Block* body;
};

struct FunBlock : public Block {
    FunBlock(const std::string& args, Block* body) : args(args), body(body) {}

    std::string toString() override {
        std::string res = std::format("Function({})\n", args);
        res += body->toString();
        res += "Endfunction()\n";
        return res;
    }

private:
    std::string args;
    Block* body;
};

struct IfBlock : public Block {
    IfBlock(std::string expr1, Block* body, Block* el = nullptr) : expr1(expr1), body(body), el(el) {}

    std::string toString() override {
        std::string res = std::format("If({})\n", expr1);
        res += body->toString();

        if (el) {
            res += "Else()\n";
            res += el->toString();
        }
        res += "Endif()\n";
        return res;
    }

private:
    std::string expr1;
    Block* el;
    Block* body;
};

struct GroupBlock : public Block {
    std::string toString() override {
        std::string res;
        for (auto it = blocks.rbegin(); it < blocks.rend(); ++it) {
            res += (*it)->toString();
        }
        return res;
    }

    void prependBlock(Block* b) {
        blocks.push_back(b);
    }

private:
    std::vector<Block*> blocks;
};

