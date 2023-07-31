#include <Constants.hpp>
#include <ExLexer.hpp>
#include <Blocks.hpp>

#include <stack>
#include <cassert>

int main() {
    ExLexer lexer;
    if (!lexer.loadFile("test.txt")) {
        return 1;
    }

    ExDictionary dict;
    if (!dict.loadDict("lexer/excmds.txt")) {
        return 2;
    }
    checkBlockConstants(dict);

    RootBlock* rootBlock = new RootBlock;
    std::stack<Block*> blocks;
    blocks.push(rootBlock);

    ExLexem lexem;
    while (lexer.lex(&lexem)) {
        int dictIdx = dict.search(lexem.name);
        Block* newBlock = nullptr;
        switch (dictIdx) {
        case IF:
            newBlock = new IfBlock(lexem.qargs);
            blocks.top()->addChild(newBlock);
            blocks.push(newBlock);
            break;

        case ELSEIF:
            if (blocks.top()->getId() != IF) {
                throw std::runtime_error("elseif without a previous if");
            }
            newBlock = new IfBlock(lexem.qargs);
            blocks.top()->cast<IfBlock>()->elseBlock = newBlock;
            blocks.top() = newBlock;
            break;

        case ELSE:
            if (blocks.top()->getId() != IF) {
                throw std::runtime_error("else without a previous if");
            }
            newBlock = new ElseBlock;
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
            newBlock = new WhileBlock(lexem.qargs);
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
            newBlock = new ForBlock(lexem.qargs);
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
            newBlock = new FunctionBlock(lexem.qargs);
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
            newBlock = new TryBlock(lexem.qargs);
            blocks.top()->addChild(newBlock);
            blocks.push(newBlock);
            break;

        case ENDTRY:
            if (blocks.top()->getId() != TRY) {
                throw std::runtime_error("endtry without a previous try");
            }
            blocks.pop();
            break;

        // TODO CATCH

        default:
            if (dictIdx < 0) {
                /* std::string cmd(lexem.name.begin, lexem.name.end); */
                /* throw std::runtime_error("unknown command + " + cmd); */
            }
            blocks.top()->addChild(new ExBlock(dictIdx, lexem));
        }
    }

    if (blocks.top() != rootBlock) {
        throw std::runtime_error("unclosed block");
    }
    blocks.pop();
    std::string res = rootBlock->toString();
    printf("%s", res.c_str());

    return 0;
}
