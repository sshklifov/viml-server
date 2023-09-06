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
        for (Block* block : blocks) {
            delete block;
        }
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

    RootBlock* build(const char* file, std::vector<Diagnostic>& errors);

    bool isBuild() const;

    ExLexer lexer; //< Holds memory for the created ExLexems and allows resolving locations
    BlockFactory blockFac; //< Holds memory for the created blocks (main ast)
    EvalFactory evalFac; //< Holds memory for the block qargs (second level ast)
};
