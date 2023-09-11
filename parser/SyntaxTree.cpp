#include "SyntaxTree.hpp"

#include <ExConstants.hpp>
#include <ExLexer.hpp>
#include <Eval.hpp>

SyntaxTree::SyntaxTree() {
    reporter.bindLocationResolver(lexer.getLocationMap());
    lexer.setDiagnosticReporter(reporter);
}

void SyntaxTree::reload(const char* str) {
    if (!lexer.reload(str)) {
        return;
    }
    blockFac.clear();
    evalFac.clear();
    reporter.clear();

    root = blockFac.create<RootBlock>();
    std::stack<Block*> blocks;
    blocks.push(root); //< Guarantees that stack is never empty

    ExLexem lexem;
    while (lexer.lex(lexem)) {
        Block* newBlock = nullptr;
        switch (lexem.exDictIdx) {
        case IF:
            newBlock = blockFac.create<IfBlock>(lexem);
            blocks.top()->addToScope(newBlock);
            blocks.push(newBlock);
            break;

        case ELSEIF:
            if (blocks.top()->getId() == IF || blocks.top()->getId() == ELSEIF) {
                newBlock = blockFac.create<ElseIfBlock>(lexem);
                if (blocks.top()->getId() == IF) {
                    blocks.top()->cast<IfBlock>()->elseIfBlock = newBlock;
                } else {
                    blocks.top()->cast<ElseIfBlock>()->elseIfBlock = newBlock;
                }
                blocks.top() = newBlock;
            } else if (blocks.top()->getId() == ELSE) {
                reporter.error("elseif after else", lexem);
            } else {
                reporter.error("elseif without if", lexem);
            }
            break;

        case ELSE:
            if (blocks.top()->getId() == IF || blocks.top()->getId() == ELSEIF) {
                newBlock = blockFac.create<ElseBlock>(lexem);
                if (blocks.top()->getId() == IF) {
                    blocks.top()->cast<IfBlock>()->elseIfBlock = newBlock;
                } else {
                    blocks.top()->cast<ElseIfBlock>()->elseIfBlock = newBlock;
                }
                blocks.top() = newBlock;
            } else if (blocks.top()->getId() == ELSE) {
                reporter.error("multiple else", lexem);
            } else {
                reporter.error("else without if", lexem);
            }
            break;

        case ENDIF:
            if (blocks.top()->getId() == IF || blocks.top()->getId() == ELSEIF || blocks.top()->getId() == ELSE) {
                blocks.pop();
            } else {
                reporter.error("endif without if", lexem);
            }
            break;

        case WHILE:
            newBlock = blockFac.create<WhileBlock>(lexem);
            blocks.top()->addToScope(newBlock);
            blocks.push(newBlock);
            break;

        case ENDWHILE:
            if (blocks.top()->getId() == WHILE) {
                blocks.pop();
            } else if (blocks.top()->getId() == FOR) {
                reporter.error("using endwhile with for", lexem);
            } else {
                reporter.error("endwhile without while", lexem);
            }
            break;

        case FOR:
            newBlock = blockFac.create<ForBlock>(lexem);
            blocks.top()->addToScope(newBlock);
            blocks.push(newBlock);
            break;

        case ENDFOR:
            if (blocks.top()->getId() == FOR) {
                blocks.pop();
            } else if (blocks.top()->getId() == WHILE) {
                reporter.error("using endfor with while", lexem);
            } else {
                reporter.error("endfor without for", lexem);
            }
            break;

        case FUNCTION:
            newBlock = blockFac.create<FunctionBlock>(lexem);
            blocks.top()->addToScope(newBlock);
            blocks.push(newBlock);
            break;

        case ENDFUNCTION:
            if (blocks.top()->getId() != FUNCTION) {
                reporter.error("endfunction not inside a function", lexem);
            } else {
                blocks.pop();
            }
            break;

        case TRY:
            newBlock = blockFac.create<TryBlock>(lexem);
            blocks.top()->addToScope(newBlock);
            blocks.push(newBlock);
            break;

         case CATCH:
            if (blocks.top()->getId() == CATCH) {
                blocks.pop(); // end catch block
            }
            if (blocks.top()->getId() == TRY) {
                newBlock = blockFac.create<CatchBlock>(lexem);
                blocks.top()->cast<TryBlock>()->catchBlocks.push_back(newBlock);
                blocks.push(newBlock);
            } else if (blocks.top()->getId() == FINALLY) {
                reporter.error("catch after finally", lexem);
            } else {
                reporter.error("catch without try", lexem);
            }
            break;

        case FINALLY:
            if (blocks.top()->getId() == CATCH) {
                blocks.pop(); // end catch block
            }
            if (blocks.top()->getId() == TRY) {
                newBlock = blockFac.create<FinallyBlock>(lexem);
                blocks.top()->cast<TryBlock>()->finally = newBlock;
                blocks.push(newBlock);
            } else if (blocks.top()->getId() == FINALLY) {
                reporter.error("multiple finally", lexem);
            } else if (blocks.top()->getId() == FINALLY) {
                reporter.error("finally without try", lexem);
            }
            break;

        case ENDTRY:
            if (blocks.top()->getId() == FINALLY || blocks.top()->getId() == CATCH) {
                blocks.pop();
            }
            if (blocks.top()->getId() != TRY) {
                reporter.error("endtry without try", lexem);
            } else {
                blocks.pop();
            }
            break;

        default:
            blocks.top()->addToScope(blockFac.create<ExBlock>(lexem));
        }
    }

    while (blocks.top() != root) {
        Block* block = blocks.top();
        int id = block->getId();
        blocks.pop();
        if (id == IF || id == ELSEIF || id == ELSE) {
            reporter.error("missing endif", block->lexem);
        } else if (id == WHILE) {
            reporter.error("missing endwhile", block->lexem);
        } else if (id == FOR) {
            reporter.error("missing endfor", block->lexem);
        } else if (id == FUNCTION) {
            reporter.error("missing endfunction", block->lexem);
        } else if (id == TRY || id == FINALLY || id == CATCH) {
            reporter.error("missing endtry", block->lexem);
        } else {
            assert(false);
        }
    }

    // Parse qargs of blocks
    struct RunEvalParse {
        RunEvalParse(EvalFactory& factory, DiagnosticReporter& reporter) :
            factory(factory), reporter(reporter) {}

        void operator()(Block* block) {
            block->evalCmd = evalEx(block->lexem, factory, reporter);
        }

    private:
        EvalFactory& factory;
        DiagnosticReporter& reporter;
    };

    RunEvalParse runner(evalFac, reporter);
    root->enumerate(runner);
}

const Vector<Diagnostic>& SyntaxTree::diagnostics() const {
    return reporter.get();
}
