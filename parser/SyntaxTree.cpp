#include "SyntaxTree.hpp"

#include <ExConstants.hpp>
#include <ExLexer.hpp>
#include <Eval.hpp>

static Diagnostic error(const ExLexer& creator, const ExLexem& lexem, const char* msg) {
    Diagnostic res;
    res.severity = Diagnostic::Error;
    res.message = msg;

    int nameStartOffset = lexem.nameOffset;
    creator.getNameLoc(lexem, res.range.start.line, res.range.start.character);
    creator.getNameEndLoc(lexem, res.range.end.line, res.range.end.character);

    return res;
}

bool SyntaxTree::isLoaded() const {
    // At least RootBlock will be created
    return !blockFac.blocks.empty();
}

void SyntaxTree::reloadFromFile(const char* file, std::vector<Diagnostic>& errors) {
    if (!lexer.reloadFromFile(file)) {
        return;
    }
    reloadStorage();
    loadImpl(errors);
}

void SyntaxTree::reloadFromString(const char* str, std::vector<Diagnostic>& errors) {
    if (!lexer.reloadFromString(str)) {
        return;
    }
    reloadStorage();
    loadImpl(errors);
}

void SyntaxTree::reloadStorage() {
    blockFac.clear();
    evalFac.clear();
    root = nullptr;
}

void SyntaxTree::loadImpl(std::vector<Diagnostic>& errors) {
    root = blockFac.create<RootBlock>();
    std::stack<Block*> blocks;
    blocks.push(root); //< Guarantees that stack is never empty

    ExLexem lexem;
    int errorSym = 0;
    while ((errorSym=lexer.lex(lexem)) != EOF) {
        if (errorSym) {
            const char* msg = "trailing characters";
            // TODO report
        }
        Block* newBlock = nullptr;
        switch (lexem.exDictIdx) {
        case IF:
            newBlock = blockFac.create<IfBlock>(lexem);
            blocks.top()->addToScope(newBlock);
            blocks.push(newBlock);
            break;

        case ELSEIF:
            if (blocks.top()->getId() == IF) {
                newBlock = blockFac.create<IfBlock>(lexem);
                blocks.top()->cast<IfBlock>()->elseBlock = newBlock;
                blocks.top() = newBlock;
            } else if (blocks.top()->getId() == ELSE) {
                const char* msg = "elseif after else";
                errors.push_back(error(lexer, lexem, msg));
            } else {
                const char* msg = "elseif without if";
                errors.push_back(error(lexer, lexem, msg));
            }
            break;

        case ELSE:
            if (blocks.top()->getId() == IF) {
                newBlock = blockFac.create<ElseBlock>(lexem);
                blocks.top()->cast<IfBlock>()->elseBlock = newBlock;
                blocks.top() = newBlock;
            } else if (blocks.top()->getId() == ELSE) {
                const char* msg = "multiple else";
                errors.push_back(error(lexer, lexem, msg));
            } else {
                const char* msg = "else without if";
                errors.push_back(error(lexer, lexem, msg));
            }
            break;

        case ENDIF:
            if (blocks.top()->getId() != IF && blocks.top()->getId() != ELSE) {
                const char* msg = "endif without if";
                errors.push_back(error(lexer, lexem, msg));
            } else {
                blocks.pop();
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
                const char* msg = "using endwhile with for";
                errors.push_back(error(lexer, lexem, msg));
            } else {
                const char* msg = "endwhile without while";
                errors.push_back(error(lexer, lexem, msg));
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
                const char* msg = "using endfor with while";
                errors.push_back(error(lexer, lexem, msg));
            } else {
                const char* msg = "endfor without for";
                errors.push_back(error(lexer, lexem, msg));
            }
            break;

        case FUNCTION:
            newBlock = blockFac.create<FunctionBlock>(lexem);
            blocks.top()->addToScope(newBlock);
            blocks.push(newBlock);
            break;

        case ENDFUNCTION:
            if (blocks.top()->getId() != FUNCTION) {
                const char* msg = "endfunction not inside a function";
                errors.push_back(error(lexer, lexem, msg));
            } else {
                blocks.pop();
            }
            break;

        case TRY:
            newBlock = blockFac.create<TryBlock>(lexem);
            blocks.top()->addToScope(newBlock);
            blocks.push(newBlock);
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
                const char* msg = "multiple finally";
                errors.push_back(error(lexer, lexem, msg));
            } else if (blocks.top()->getId() == FINALLY) {
                const char* msg = "finally without try";
                errors.push_back(error(lexer, lexem, msg));
            }
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
                const char* msg = "catch after finally";
                errors.push_back(error(lexer, lexem, msg));
            } else {
                const char* msg = "catch without try";
                errors.push_back(error(lexer, lexem, msg));
            }
            break;

        case ENDTRY:
            if (blocks.top()->getId() != TRY) {
                const char* msg = "endtry without try";
                errors.push_back(error(lexer, lexem, msg));
            } else {
                blocks.pop();
            }
            break;

        default:
            if (lexem.exDictIdx < 0) { // TODO handle in lexer?
                const char* msg = "not an editor command";
                errors.push_back(error(lexer, lexem, msg));
            }
            // Add it anyway
            if (!lexem.name.empty()) {
                blocks.top()->addToScope(blockFac.create<ExBlock>(lexem));
            }
        }
    }

    while (blocks.top() != root) {
        Block* block = blocks.top();
        int id = block->getId();
        blocks.pop();

        const char* msg = nullptr;
        if (id == IF || id == ELSEIF || id == ELSE) {
            msg = "missing endif";
        } else if (id == WHILE) {
            msg = "missing endwhile";
        } else if (id == FOR) {
            msg = "missing endfor";
        } else if (id == FUNCTION) {
            msg = "missing endfunction";
        } else if (id == TRY || id == FINALLY || id == CATCH) {
            msg = "missing endtry";
        }

        assert(msg);
        if (msg) {
            errors.push_back(error(lexer, block->lexem, msg));
        }
    }

    // Parse qargs of blocks

    struct RunEvalParse {
        RunEvalParse(SyntaxTree& ast, std::vector<Diagnostic>& digs) : ast(ast), digs(digs) {}

        void operator()(Block* block) {
            evalParseBlock(*block, ast, digs);
        }

    private:
        SyntaxTree& ast;
        std::vector<Diagnostic>& digs;
    } runEvalParse(*this, errors);

    root->enumerate(runEvalParse);
}
