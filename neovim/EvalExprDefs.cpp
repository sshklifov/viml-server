#include "EvalExprDefs.hpp"


// Regular symbol construction
SymbolExpr* EvalFactory::create(const char* begin, const char* end) {
    if (evaluate == EVAL_ALL || evaluate == EVAL_SYMBOLS_ONLY) {
        SymbolExpr* res = new SymbolExpr(begin, end);
        exprs.emplace(res);
        return res;
    } else {
        return nullptr;
    }
}

// Curly expansion construction TODO change or remove
SymbolExpr* EvalFactory::create(const char* begin, const char* end, FStr pat, EvalFactory&& dep) {
    if (evaluate == EVAL_ALL || evaluate == EVAL_SYMBOLS_ONLY) {
        // TODO
        /* SymbolExpr* res = new SymbolExpr(begin, end, pat, dep); */
        /* exprs.emplace(res); */
        /* return res; */
        return nullptr;
    } else {
        return nullptr;
    }
}
