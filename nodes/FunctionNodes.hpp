#pragma once

#include "BaseNode.hpp"
#include "SkipFuncs.hpp"

// flags used in uf_flags
#define FC_ABORT    0x01          // abort function on error
#define FC_RANGE    0x02          // function accepts range
#define FC_DICT     0x04          // Dict function, uses "self"
#define FC_CLOSURE  0x08          // closure, uses outer scope variables
#define FC_DELETED  0x10          // :delfunction used while uf_refcount > 0
#define FC_REMOVED  0x20          // function redefined while uf_refcount > 0
#define FC_SANDBOX  0x40          // function defined in the sandbox
#define FC_DEAD     0x80          // function kept only for reference to dfunc
#define FC_EXPORT   0x100         // "export def Func()"
#define FC_NOARGS   0x200         // no a: variables in lambda
#define FC_VIM9     0x400         // defined in vim9 script file
#define FC_LUAREF  0x800          // luaref callback

// TODO function /pat/ | endfunction !!!!!!!!!!!!!!!

struct FunctionNode : public GroupNode {
    FunctionNode(const ExLexem& lexem) : GroupNode(lexem) { reset(); }

    void reset() {
        nameBegin = nullptr;
        nameEnd = nullptr;
        name = nullptr;
        flags = 0;

        f.clear();
        fargs.clear();
    }

    const char* parseArgs(BoundReporter& rep) override {
        const char* p = lex.qargs;

        // ":function" without argument: list functions.
        if (ends_excmd(*p)) {
            return p;
        }

        try {
            // ":function /pat": list functions matching pattern.
            if (*p == '/') {
                p = skip_regexp(p + 1, '/');
                if (*p == '/') {
                    p++;
                }
                return p;
            }

            // Get the function name.  There are these situations:
            // func        function name
            //             "name" == func, "fudi.fd_dict" == NULL
            // dict.func   new dictionary entry
            //             "name" == NULL, "fudi.fd_dict" set,
            //             "fudi.fd_di" == NULL, "fudi.fd_newkey" == func
            // dict.func   existing dict entry with a Funcref
            //             "name" == func, "fudi.fd_dict" set,
            //             "fudi.fd_di" set, "fudi.fd_newkey" == NULL
            // dict.func   existing dict entry that's not a Funcref
            //             "name" == NULL, "fudi.fd_dict" set,
            //             "fudi.fd_di" set, "fudi.fd_newkey" == NULL
            // s:func      script-local function name
            // g:func      global function name, same as "func"
            nameBegin = p;
            name = get_var_indexed(p, f);
            nameEnd = p;
            p = skipwhite(p);
            if (*p != '(') {
                if (ends_excmd(*p)) {
                    return p;
                } else {
                    throw msg(p, "Missing '('");
                }
            }
            p = skipwhite(p + 1);
            fargs = get_function_args(p, false, f);
            p++;
        } catch (msg& m) {
            rep.error(m);
            reset();
            return nullptr;
        }

        // find extra arguments "range", "dict", "abort" and "closure"
        for (;;) {
            p = skipwhite(p);
            if (strncmp(p, "range", 5) == 0) {
                flags |= FC_RANGE;
                p += 5;
            } else if (strncmp(p, "dict", 4) == 0) {
                flags |= FC_DICT;
                p += 4;
            } else if (strncmp(p, "abort", 5) == 0) {
                flags |= FC_ABORT;
                p += 5;
            } else if (strncmp(p, "closure", 7) == 0) {
                flags |= FC_CLOSURE;
                p += 7;
                // TODO check if toplevel
            } else {
                break;
            }
        }
        return p;
    }

    static const int id = CMD_function;

    const char* nameBegin; // TODO
    const char* nameEnd; // TODO
    EvalExpr* name;
    Vector<SymbolExpr*> fargs;
    int flags;
};
