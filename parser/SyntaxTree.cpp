#include "SyntaxTree.hpp"
#include "Constants.hpp"

#include <ExDictionary.hpp>
#include <ExLexer.hpp>

static bool checkBlockConstants(const ExDictionary& dict) {
    std::string lowercase;
    std::vector<int> values;

    int okay = true;

#define COMPUTE_COMMAND_VALUE(name, val)              \
    lowercase = #name;                                \
    for (int i = 0; i < lowercase.length(); ++i) {    \
        lowercase[i] = tolower(lowercase[i]);         \
    }                                                 \
    values.push_back(dict.search(lowercase.c_str())); \
    okay &= (values.back() == val);                   \

    FOR_EACH_COMMAND(COMPUTE_COMMAND_VALUE);

#undef COMPUTE_COMMAND_VALUE

    if (okay) {
        return true;
    }

    int i = 0;

#define PRINT_COMMAND_VALUE(name, val) printf("    code(%s, %d) \\\n", #name, values[i++]);

    FOR_EACH_COMMAND(PRINT_COMMAND_VALUE)

#undef PRINT_COMMAND_VALUE

    return okay;
}

static Diagnostic error(const ExLexer& creator, const ExLexem& lexem, const char* msg) {
    Diagnostic res;
    res.severity = Diagnostic::Error;
    res.message = msg;

    int nameStartOffset = lexem.nameOffset;
    creator.resolveNameLoc(lexem, res.range.start.line, res.range.start.character);
    creator.resolveNameEndLoc(lexem, res.range.end.line, res.range.end.character);

    return res;
}

bool SyntaxTree::isBuild() const {
    return !factory.allocatedBlocks.empty();
}

RootBlock* SyntaxTree::build(const char* file, std::vector<Diagnostic>& errors) {
    if (isBuild()) {
        assert(false);
        return nullptr;
    }

    if (!lexer.loadFile(file)) {
        return nullptr;
    }

    const ExDictionary& dict = ExDictionary::getSingleton();
    if (!dict.isLoaded()) {
        return nullptr;
    }

    assert(checkBlockConstants(dict));

    RootBlock* root = factory.create<RootBlock>();
    std::stack<Block*> blocks;
    blocks.push(root); //< Guarantees that stack is never empty

    ExLexem lexem;
    int errorSym = 0;
    while ((errorSym=lexer.lex(lexem)) != EOF) {
        if (errorSym) {
            const char* msg = "trailing characters";
            // TODO report
        }
        int dictIdx = dict.search(lexem.name);
        Block* newBlock = nullptr;
        switch (dictIdx) {
        case IF:
            newBlock = factory.create<IfBlock>(lexem);
            blocks.top()->addToScope(newBlock);
            blocks.push(newBlock);
            break;

        case ELSEIF:
            if (blocks.top()->getId() == IF) {
                newBlock = factory.create<IfBlock>(lexem);
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
                newBlock = factory.create<ElseBlock>(lexem);
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
            newBlock = factory.create<WhileBlock>(lexem);
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
            newBlock = factory.create<ForBlock>(lexem);
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
            newBlock = factory.create<FunctionBlock>(lexem);
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
            newBlock = factory.create<TryBlock>(lexem);
            blocks.top()->addToScope(newBlock);
            blocks.push(newBlock);
            break;

        case FINALLY:
            if (blocks.top()->getId() == CATCH) {
                blocks.pop(); // end catch block
            }
            if (blocks.top()->getId() == TRY) {
                newBlock = factory.create<FinallyBlock>(lexem);
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
                newBlock = factory.create<CatchBlock>(lexem);
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
            if (dictIdx < 0) {
                const char* msg = "not an editor command";
                errors.push_back(error(lexer, lexem, msg));
            }
            // Add it anyway
            if (!lexem.name.empty()) {
                blocks.top()->addToScope(factory.create<ExBlock>(lexem, dictIdx));
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

    lexer.unloadFile();
    return root;
}
