#include "SyntaxTree.hpp"

#include <Stack.hpp>
#include <ExCmdsDefs.hpp>
#include <ExCmdsEnum.hpp>

void SyntaxTree::reload(const char* str) {
    if (!lexer.reload(str)) {
        return;
    }
    f.clear();
    rep.clear();

    root = f.create<RootNode>();
    Stack<GroupNode*> nodes;
    nodes.emplace(root); //< Guarantees that stack is never empty

    ExLexem lexem;
    BoundReporter boundRep(rep, lexem);
    while (lexer.lexNext(lexem, rep)) {
        GroupNode* grpNode = nullptr;
        int not_done = 0;
        switch (lexem.cmdidx) {
        case CMD_if:
            grpNode = f.create<IfNode>(lexem);
            nodes.top()->children.emplace(grpNode);
            nodes.emplace(grpNode);
            break;

        case CMD_elseif:
            if (nodes.top()->getId() == CMD_if || nodes.top()->getId() == CMD_elseif) {
                grpNode = f.create<ElseIfNode>(lexem);
                if (nodes.top()->getId() == CMD_if) {
                    nodes.top()->cast<IfNode>()->elseIfNode = grpNode;
                } else {
                    nodes.top()->cast<ElseIfNode>()->elseIfNode = grpNode;
                }
                nodes.top() = grpNode;
            } else if (nodes.top()->getId() == CMD_else) {
                boundRep.errorName("elseif after else");
            } else {
                boundRep.errorName("elseif without if");
            }
            break;

        case CMD_else:
            if (nodes.top()->getId() == CMD_if || nodes.top()->getId() == CMD_elseif) {
                grpNode = f.create<ElseNode>(lexem);
                if (nodes.top()->getId() == CMD_if) {
                    nodes.top()->cast<IfNode>()->elseIfNode = grpNode;
                } else {
                    nodes.top()->cast<ElseIfNode>()->elseIfNode = grpNode;
                }
                nodes.top() = grpNode;
            } else if (nodes.top()->getId() == CMD_else) {
                boundRep.errorName("multiple else");
            } else {
                boundRep.errorName("else without if");
            }
            break;

        case CMD_endif:
            if (nodes.top()->getId() == CMD_if || nodes.top()->getId() == CMD_elseif || nodes.top()->getId() == CMD_else) {
                nodes.pop();
            } else {
                boundRep.errorName("endif without if");
            }
            break;

        case CMD_while:
            grpNode = f.create<WhileNode>(lexem);
            nodes.top()->children.emplace(grpNode);
            nodes.emplace(grpNode);
            break;

        case CMD_endwhile:
            if (nodes.top()->getId() == CMD_while) {
                nodes.pop();
            } else if (nodes.top()->getId() == CMD_for) {
                boundRep.errorName("using endwhile with for");
            } else {
                boundRep.errorName("endwhile without while");
            }
            break;

        case CMD_for:
            grpNode = f.create<ForNode>(lexem);
            nodes.top()->children.emplace(grpNode);
            nodes.emplace(grpNode);
            break;

        case CMD_endfor:
            if (nodes.top()->getId() == CMD_for) {
                nodes.pop();
            } else if (nodes.top()->getId() == CMD_while) {
                boundRep.errorName("using endfor with while");
            } else {
                boundRep.errorName("endfor without for");
            }
            break;

        case CMD_function:
            grpNode = f.create<FunctionNode>(lexem);
            nodes.top()->children.emplace(grpNode);
            // Don't add to stack yet, might be function listing
            break;

        case CMD_endfunction:
            if (nodes.top()->getId() != CMD_function) {
                boundRep.errorName("endfunction not inside a function");
            } else {
                nodes.pop();
            }
            break;

        case CMD_try:
            grpNode = f.create<TryNode>(lexem);
            nodes.top()->children.emplace(grpNode);
            nodes.emplace(grpNode);
            break;

         case CMD_catch:
            if (nodes.top()->getId() == CMD_catch) {
                nodes.pop(); // end catch node
            }
            if (nodes.top()->getId() == CMD_try) {
                grpNode = f.create<CatchNode>(lexem);
                nodes.top()->cast<TryNode>()->catchNodes.emplace(grpNode);
                nodes.emplace(grpNode);
            } else if (nodes.top()->getId() == CMD_finally) {
                boundRep.errorName("catch after finally");
            } else {
                boundRep.errorName("catch without try");
            }
            break;

        case CMD_finally:
            if (nodes.top()->getId() == CMD_catch) {
                nodes.pop(); // end catch node
            }
            if (nodes.top()->getId() == CMD_try) {
                grpNode = f.create<FinallyNode>(lexem);
                nodes.top()->cast<TryNode>()->finally = grpNode;
                nodes.emplace(grpNode);
            } else if (nodes.top()->getId() == CMD_finally) {
                boundRep.errorName("multiple finally");
            } else if (nodes.top()->getId() == CMD_finally) {
                boundRep.errorName("finally without try");
            }
            break;

        case CMD_endtry:
            if (nodes.top()->getId() == CMD_finally || nodes.top()->getId() == CMD_catch) {
                nodes.pop();
            }
            if (nodes.top()->getId() != CMD_try) {
                boundRep.errorName("endtry without try");
            } else {
                nodes.pop();
            }
            break;

        default:
            not_done = 1;
            break;
        }

        if (grpNode) {
            grpNode->parse(rep, lexem.nextcmd);
            // After being parsed, check if function node is a true function
            if (FunctionNode* fnode = grpNode->cast<FunctionNode>()) {
                if (fnode->name) {
                    nodes.emplace(grpNode);
                }
            }
        } else if (not_done) {
            ex_func_T func = cmdnames[lexem.cmdidx].cmd_func;
            if (func) {
                BaseNode* exNode = (BaseNode*)func(&lexem);
                if (exNode) {
                    f.add(exNode);
                    nodes.top()->children.emplace(exNode);
                    exNode->parse(rep, lexem.nextcmd);
                }
            }
        }
    }

    while (nodes.top() != root) {
        GroupNode* node = nodes.top();
        int id = node->getId();
        nodes.pop();
        if (id == CMD_if || id == CMD_elseif || id == CMD_else) {
            boundRep.errorName("missing endif");
        } else if (id == CMD_while) {
            boundRep.errorName("missing endwhile");
        } else if (id == CMD_for) {
            boundRep.errorName("missing endfor");
        } else if (id == CMD_function) {
            boundRep.errorName("missing endfunction");
        } else if (id == CMD_try || id == CMD_finally || id == CMD_catch) {
            boundRep.errorName("missing endtry");
        } else {
            assert(false);
        }
    }
}

void SyntaxTree::symbols(int hierarchy, Vector<DocumentSymbol>& syms) {
    struct EnumSymbolsHierarchy {
        EnumSymbolsHierarchy(Vector<DocumentSymbol>& res) : res(res) {}

        int operator()(BaseNode* node) {
            FunctionNode* fnode = node->cast<FunctionNode>();
            if (fnode) {
                if (fnode->name) {
                    DocumentSymbol& sym = res.newElem();
                    sym.name = FStr(fnode->nameBegin, fnode->nameEnd);
                    sym.kind = SymbolKind::Function;
                    sym.range = fnode->lex.locator.resolve(fnode->nameBegin, fnode->nameEnd);
                    EnumSymbolsHierarchy cb(sym.children);
                    for (BaseNode* child : fnode->children) {
                        cb(child);
                    }
                    return BaseNode::ENUM_PRUNE;
                }
            }
            return BaseNode::ENUM_NEXT;
        }

    private:
        Vector<DocumentSymbol>& res;
    };

    if (hierarchy) {
        EnumSymbolsHierarchy cb(syms);
        root->enumerate(cb);
    } else {
        for (BaseNode* node : f) {
            FunctionNode* fnode = node->cast<FunctionNode>();
            if (fnode) {
                if (fnode->name) {
                    DocumentSymbol& sym = syms.newElem();
                    sym.name = FStr(fnode->nameBegin, fnode->nameEnd);
                    sym.kind = SymbolKind::Function;
                    sym.range = fnode->lex.locator.resolve(fnode->nameBegin, fnode->nameEnd);
                }
            }
        }
    }
}

const Vector<Diagnostic>& SyntaxTree::diagnostics() const {
    return rep.get();
}

BaseNode* SyntaxTree::findNode(const Position& pos) {
    return root->findNode(pos);
}
