#pragma once

#include <Nodes.hpp>
#include <ExLexer.hpp>
#include <DiagnosticReporter.hpp>

struct NodeFactory {
    NodeFactory() = default;
    NodeFactory(const NodeFactory&) = delete;
    NodeFactory(NodeFactory&&) = delete;

    ~NodeFactory() {
        clear();
    }

    void clear() {
        for (BaseExpr* node : nodes) {
            delete node;
        }
        nodes.clear();
    }

    template <typename T, typename ... Args>
    T* create(Args&&... args) {
        static_assert(std::is_base_of<BaseExpr, T>::value, "Bad template argument");

        T* res = new T(std::forward<Args>(args)...);
        nodes.emplace(res);
        return res;
    }

    Vector<BaseExpr*> nodes;
};

struct SyntaxTree {
    SyntaxTree() = default;
    SyntaxTree(const SyntaxTree&) = delete;
    SyntaxTree(SyntaxTree&&) = delete;

    void reload(const char* str);
    const Vector<Diagnostic>& diagnostics() const;

public:
    DiagnosticReporter rep; //< Diagnostics found during parse of AST
    ExLexer lexer; //< Holds memory for the created ExLexems and allows resolving locations
    NodeFactory factory; //< Holds memory for the created nodes
    RootNode* root; //< AST root
};
