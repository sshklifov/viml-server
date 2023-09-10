#pragma once

#include <Blocks.hpp>
#include <Diagnostics.hpp>
#include <EvalFactory.hpp>

#include <type_traits>
#include <stack>
#include <cassert>

struct BlockFactory {
    BlockFactory() = default;
    BlockFactory(const BlockFactory&) = delete;
    BlockFactory(BlockFactory&&) = delete;

    ~BlockFactory() {
        clear();
    }

    void clear() {
        for (Block* block : blocks) {
            delete block;
        }
        blocks.clear();
    }

    template <typename T, typename ... Args>
    T* create(Args&&... args) {
        static_assert(std::is_base_of<Block, T>::value, "Bad template argument");

        T* res = new T(std::forward<Args>(args)...);
        blocks.push_back(res);
        return res;
    }

    std::vector<Block*> blocks;
};

struct SyntaxTree {
    SyntaxTree() = default;
    SyntaxTree(const SyntaxTree&) = delete;
    SyntaxTree(SyntaxTree&&) = delete;

    void reload(const char* str, std::vector<Diagnostic>& errors);
    bool isLoaded() const;
private:
    void reloadStorage();

public:
    ExLexer lexer; //< Holds memory for the created ExLexems and allows resolving locations
    BlockFactory blockFac; //< Holds memory for the created blocks (main ast)
    EvalFactory evalFac; //< Holds memory for command qargs (second level ast)
    RootBlock* root; //< AST root
};
