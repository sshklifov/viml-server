#include <Constants.hpp>
#include <ExLexer.hpp>
#include <Blocks.hpp>

#include <type_traits>
#include <stack>

#include <cassert>

// TODO
#include "../lsp/Diagnostics.hpp"

struct BlockFactory {
    BlockFactory() = default;
    BlockFactory(const BlockFactory&) = delete;
    BlockFactory(BlockFactory&&) = delete;

    ~BlockFactory() {
        for (Block* block : allocatedBlocks) {
            delete block;
        }
    }

    template <typename T, typename ... Args>
    T* create(Args&&... args) {
        static_assert(std::is_base_of<Block, T>::value, "Bad template argument");

        T* res = new T(std::forward<Args>(args)...);
        allocatedBlocks.push_back(res);
        return res;
    }

    std::vector<Block*> allocatedBlocks;
};

struct SyntaxTree {
    SyntaxTree() = default;
    SyntaxTree(const SyntaxTree&) = delete;
    SyntaxTree(SyntaxTree&&) = delete;

    // TODO return and stuff
    int build(const char* file) {
        ExLexer lexer;
        if (!lexer.loadFile(file)) {
            return 1;
        }

        ExDictionary dict;
        if (!dict.loadDict("/home/stef/viml-server/lexer/excmds.txt")) {
            return 2;
        }

        assert(checkBlockConstants(dict));

        root = factory.create<RootBlock>();
        std::stack<Block*> blocks;
        blocks.push(root);

        ExLexem lexem;
        while (lexer.lex(&lexem)) {
            int dictIdx = dict.search(lexem.name);
            Block* newBlock = nullptr;
            switch (dictIdx) {
            case IF:
                newBlock = factory.create<IfBlock>(lexem.qargs);
                blocks.top()->addChild(newBlock);
                blocks.push(newBlock);
                break;

            case ELSEIF:
                if (blocks.top()->getId() != IF) {
                    throw std::runtime_error("elseif without a previous if");
                }
                newBlock = factory.create<IfBlock>(lexem.qargs);
                blocks.top()->cast<IfBlock>()->elseBlock = newBlock;
                blocks.top() = newBlock;
                break;

            case ELSE:
                if (blocks.top()->getId() != IF) {
                    throw std::runtime_error("else without a previous if");
                }
                newBlock = factory.create<ElseBlock>();
                blocks.top()->cast<IfBlock>()->elseBlock = newBlock;
                blocks.top() = newBlock;
                break;

            case ENDIF:
                if (blocks.top()->getId() != IF && blocks.top()->getId() != ELSE) {
                    throw std::runtime_error("endif without a previous if/else");
                }
                blocks.pop();
                break;

            case WHILE:
                newBlock = factory.create<WhileBlock>(lexem.qargs);
                blocks.top()->addChild(newBlock);
                blocks.push(newBlock);
                break;

            case ENDWHILE:
                if (blocks.top()->getId() != WHILE) {
                    throw std::runtime_error("endwhile without a previous while");
                }
                blocks.pop();
                break;

            case FOR:
                newBlock = factory.create<ForBlock>(lexem.qargs);
                blocks.top()->addChild(newBlock);
                blocks.push(newBlock);
                break;

            case ENDFOR:
                if (blocks.top()->getId() != FOR) {
                    throw std::runtime_error("endfor without a previous for");
                }
                blocks.pop();
                break;

            case FUNCTION:
                newBlock = factory.create<FunctionBlock>(lexem.qargs);
                blocks.top()->addChild(newBlock);
                blocks.push(newBlock);
                break;

            case ENDFUNCTION:
                if (blocks.top()->getId() != FUNCTION) {
                    throw std::runtime_error("endfunction without a previous function");
                }
                blocks.pop();
                break;

            case TRY:
                newBlock = factory.create<TryBlock>(lexem.qargs);
                blocks.top()->addChild(newBlock);
                blocks.push(newBlock);
                break;

            case ENDTRY:
                if (blocks.top()->getId() != TRY) {
                    throw std::runtime_error("endtry without a previous try");
                }
                blocks.pop();
                break;

            case CATCH:
                if (blocks.top()->getId() == CATCH) {
                    blocks.pop();
                }
                if (blocks.top()->getId() == TRY) {
                    newBlock = factory.create<CatchBlock>(lexem.qargs);
                    blocks.top()->cast<TryBlock>()->catchBlocks.push_back(newBlock);
                    blocks.push(newBlock);
                } else {
                    throw std::runtime_error("catch without a previous try");
                }
                break;

            default:
                if (dictIdx < 0) {
                    /* std::string cmd(lexem.name.begin, lexem.name.end); */
                    /* throw std::runtime_error("unknown command + " + cmd); */
                }
                blocks.top()->addChild(factory.create<ExBlock>(dictIdx, lexem));
            }
        }

        if (blocks.top() != root) {
            throw std::runtime_error("unclosed block");
        }
        /* blocks.pop(); */
        /* std::string res = root->toString(); */
        /* printf("%s", res.c_str()); */
        return 0;
    }

private:
    RootBlock* root;
    BlockFactory factory;
    std::vector<Diagnostic> errors;
};

int main() {
    SyntaxTree ast;
    ast.build("/home/stef/viml-server/test.txt");
}
