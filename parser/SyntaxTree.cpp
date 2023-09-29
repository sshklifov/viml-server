#include "SyntaxTree.hpp"

#include <Stack.hpp>
#include <ExCmdsEnum.hpp>

void SyntaxTree::reload(const char* str) {
    if (!lexer.reload(str)) {
        return;
    }
    factory.clear();
    rep.clear();

    root = factory.create<RootNode>();
    Stack<GroupNode*> nodes;
    nodes.emplace(root); //< Guarantees that stack is never empty

    const char* nextcmd = nullptr;
    ExLexem lexem;
    while (lexer.lexNext(lexem, nextcmd, rep)) {
        GroupNode* node = nullptr;
        switch (lexem.cmdidx) {
        case CMD_if:
            node = factory.create<IfNode>(lexem);
            nodes.top()->body.emplace(node);
            nodes.emplace(node);
            break;

        case CMD_elseif:
            if (nodes.top()->getId() == CMD_if || nodes.top()->getId() == CMD_elseif) {
                node = factory.create<ElseIfNode>(lexem);
                if (nodes.top()->getId() == CMD_if) {
                    nodes.top()->cast<IfNode>()->elseIfNode = node;
                } else {
                    nodes.top()->cast<ElseIfNode>()->elseIfNode = node;
                }
                nodes.top() = node;
            } else if (nodes.top()->getId() == CMD_else) {
                rep.error("elseif after else", lexem);
            } else {
                rep.error("elseif without if", lexem);
            }
            break;

        case CMD_else:
            if (nodes.top()->getId() == CMD_if || nodes.top()->getId() == CMD_elseif) {
                node = factory.create<ElseNode>(lexem);
                if (nodes.top()->getId() == CMD_if) {
                    nodes.top()->cast<IfNode>()->elseIfNode = node;
                } else {
                    nodes.top()->cast<ElseIfNode>()->elseIfNode = node;
                }
                nodes.top() = node;
            } else if (nodes.top()->getId() == CMD_else) {
                rep.error("multiple else", lexem);
            } else {
                rep.error("else without if", lexem);
            }
            break;

        case CMD_endif:
            if (nodes.top()->getId() == CMD_if || nodes.top()->getId() == CMD_elseif || nodes.top()->getId() == CMD_else) {
                nodes.pop();
            } else {
                rep.error("endif without if", lexem);
            }
            break;

        case CMD_while:
            node = factory.create<WhileNode>(lexem);
            nodes.top()->body.emplace(node);
            nodes.emplace(node);
            break;

        case CMD_endwhile:
            if (nodes.top()->getId() == CMD_while) {
                nodes.pop();
            } else if (nodes.top()->getId() == CMD_for) {
                rep.error("using endwhile with for", lexem);
            } else {
                rep.error("endwhile without while", lexem);
            }
            break;

        case CMD_for:
            node = factory.create<ForNode>(lexem);
            nodes.top()->body.emplace(node);
            nodes.emplace(node);
            break;

        case CMD_endfor:
            if (nodes.top()->getId() == CMD_for) {
                nodes.pop();
            } else if (nodes.top()->getId() == CMD_while) {
                rep.error("using endfor with while", lexem);
            } else {
                rep.error("endfor without for", lexem);
            }
            break;

        case CMD_function:
            node = factory.create<FunctionNode>(lexem);
            nodes.top()->body.emplace(node);
            nodes.emplace(node);
            break;

        case CMD_endfunction:
            if (nodes.top()->getId() != CMD_function) {
                rep.error("endfunction not inside a function", lexem);
            } else {
                nodes.pop();
            }
            break;

        case CMD_try:
            node = factory.create<TryNode>(lexem);
            nodes.top()->body.emplace(node);
            nodes.emplace(node);
            break;

         case CMD_catch:
            if (nodes.top()->getId() == CMD_catch) {
                nodes.pop(); // end catch node
            }
            if (nodes.top()->getId() == CMD_try) {
                node = factory.create<CatchNode>(lexem);
                nodes.top()->cast<TryNode>()->catchNodes.emplace(node);
                nodes.emplace(node);
            } else if (nodes.top()->getId() == CMD_finally) {
                rep.error("catch after finally", lexem);
            } else {
                rep.error("catch without try", lexem);
            }
            break;

        case CMD_finally:
            if (nodes.top()->getId() == CMD_catch) {
                nodes.pop(); // end catch node
            }
            if (nodes.top()->getId() == CMD_try) {
                node = factory.create<FinallyNode>(lexem);
                nodes.top()->cast<TryNode>()->finally = node;
                nodes.emplace(node);
            } else if (nodes.top()->getId() == CMD_finally) {
                rep.error("multiple finally", lexem);
            } else if (nodes.top()->getId() == CMD_finally) {
                rep.error("finally without try", lexem);
            }
            break;

        case CMD_endtry:
            if (nodes.top()->getId() == CMD_finally || nodes.top()->getId() == CMD_catch) {
                nodes.pop();
            }
            if (nodes.top()->getId() != CMD_try) {
                rep.error("endtry without try", lexem);
            } else {
                nodes.pop();
            }
            break;

        default:
            nodes.top()->body.emplace(factory.create<ExNode>(lexem));
        }

        if (node) {
            node->parse(rep, nextcmd);
        }
    }

    while (nodes.top() != root) {
        GroupNode* node = nodes.top();
        int id = node->getId();
        nodes.pop();
        if (id == CMD_if || id == CMD_elseif || id == CMD_else) {
            rep.error("missing endif", node->lex);
        } else if (id == CMD_while) {
            rep.error("missing endwhile", node->lex);
        } else if (id == CMD_for) {
            rep.error("missing endfor", node->lex);
        } else if (id == CMD_function) {
            rep.error("missing endfunction", node->lex);
        } else if (id == CMD_try || id == CMD_finally || id == CMD_catch) {
            rep.error("missing endtry", node->lex);
        } else {
            assert(false);
        }
    }
}

const Vector<Diagnostic>& SyntaxTree::diagnostics() const {
    return rep.get();
}
