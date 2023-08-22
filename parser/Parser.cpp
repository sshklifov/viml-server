#include "Parser.hpp"
#include "Constants.hpp"

#include <ExDictionary.hpp>
#include <ExLexer.hpp>

bool checkBlockConstants(const ExDictionary& dict) {
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

RootBlock* SyntaxTree::build(const char* file, std::vector<Diagnostic>& errors) {
	ExLexer lexer;
	if (!lexer.loadFile(file)) {
		return nullptr;
	}

	ExDictionary dict;
	if (!dict.loadDict("/home/stef/viml-server/lexer/excmds.txt")) {
		return nullptr;
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
				const char* msg = "elseif without a previous if";
				errors.emplace_back(Diagnostic{lexem.getNameRange(), Diagnostic::Error, msg});
			} else {
				newBlock = factory.create<IfBlock>(lexem.qargs);
				blocks.top()->cast<IfBlock>()->elseBlock = newBlock;
				blocks.top() = newBlock;
			}
			break;

		case ELSE:
			if (blocks.top()->getId() != IF) {
				const char* msg = "elseif without a previous if";
				errors.emplace_back(Diagnostic{lexem.getNameRange(), Diagnostic::Error, msg});
			} else {
				newBlock = factory.create<ElseBlock>();
				blocks.top()->cast<IfBlock>()->elseBlock = newBlock;
				blocks.top() = newBlock;
			}
			break;

		case ENDIF:
			if (blocks.top()->getId() != IF && blocks.top()->getId() != ELSE) {
				const char* msg = "endif without a previous if/else";
				errors.emplace_back(Diagnostic{lexem.getNameRange(), Diagnostic::Error, msg});
			} else {
				blocks.pop();
			}
			break;

		case WHILE:
			newBlock = factory.create<WhileBlock>(lexem.qargs);
			blocks.top()->addChild(newBlock);
			blocks.push(newBlock);
			break;

		case ENDWHILE:
			if (blocks.top()->getId() != WHILE) {
				const char* msg = "endwhile without a previous while";
				errors.emplace_back(Diagnostic{lexem.getNameRange(), Diagnostic::Error, msg});
			} else {
				blocks.pop();
			}
			break;

		case FOR:
			newBlock = factory.create<ForBlock>(lexem.qargs);
			blocks.top()->addChild(newBlock);
			blocks.push(newBlock);
			break;

		case ENDFOR:
			if (blocks.top()->getId() != FOR) {
				const char* msg = "endfor without a previous for";
				errors.emplace_back(Diagnostic{lexem.getNameRange(), Diagnostic::Error, msg});
			} else {
				blocks.pop();
			}
			break;

		case FUNCTION:
			newBlock = factory.create<FunctionBlock>(lexem.qargs);
			blocks.top()->addChild(newBlock);
			blocks.push(newBlock);
			break;

		case ENDFUNCTION:
			if (blocks.top()->getId() != FUNCTION) {
				const char* msg = "endfunction without a previous function";
				errors.emplace_back(Diagnostic{lexem.getNameRange(), Diagnostic::Error, msg});
			} else {
				blocks.pop();
			}
			break;

		case TRY:
			newBlock = factory.create<TryBlock>(lexem.qargs);
			blocks.top()->addChild(newBlock);
			blocks.push(newBlock);
			break;

		case ENDTRY:
			if (blocks.top()->getId() != TRY) {
				const char* msg = "endtry without a previous try";
				errors.emplace_back(Diagnostic{lexem.getNameRange(), Diagnostic::Error, msg});
			} else {
				blocks.pop();
			}
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
				const char* msg = "catch without a previous try";
				errors.emplace_back(Diagnostic{lexem.getNameRange(), Diagnostic::Error, msg});
			}
			break;

		default:
			if (dictIdx < 0) {
				const char* msg = "unknown command";
				errors.emplace_back(Diagnostic{lexem.getNameRange(), Diagnostic::Error, msg});
			}
			// Add it anyway
			blocks.top()->addChild(factory.create<ExBlock>(dictIdx, lexem.name, lexem.qargs));
		}
	}

	if (blocks.top() != root) {
		const char* msg = "unclosed block";
		errors.push_back(Diagnostic{lexem.getNameRange(), Diagnostic::Error, msg});
	}
	return root;
}
