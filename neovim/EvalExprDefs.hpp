#pragma once

#include <Vector.hpp>
#include <FStr.hpp>

#include <NeovimBase.hpp>

struct EvalExpr {
    enum {
        expr_ternary,
        expr_logic,
        expr_compare,
        expr_arith,
        expr_prefix,
        expr_index,
        expr_index2,
        expr_invoke,
        expr_symbol,
        expr_literal,
        expr_list,
        expr_dict,
        expr_nest,
        expr_lambda
    };

    virtual ~EvalExpr() {}

    virtual int getId() = 0;
};

struct SymbolExpr;

struct EvalFactory {
    enum Mode { EVAL_ALL, EVAL_SYMBOLS_ONLY, EVAL_NONE };

    EvalFactory() : evaluate(EVAL_ALL) {}
    EvalFactory(Mode mode) : evaluate(mode)  {}

    EvalFactory(const EvalFactory&) = delete;

    ~EvalFactory() {
        clear();
    }

    void clear() {
        for (EvalExpr* expr : exprs) {
            delete expr;
        }
        exprs.clear();
    }

    SymbolExpr* findSymbol(const char* p);

    int count() const { return exprs.count(); }
    EvalExpr* operator[](int i) { return exprs[i]; }

    Mode setEvaluate(Mode mode) {
        Mode old = evaluate;
        evaluate = mode;
        return old;
    }

    template <typename T, typename ... Args>
    T* create(Args&&... args) {
        if (evaluate == EVAL_ALL) {
            T* res = new T(std::forward<Args>(args)...);
            exprs.emplace(res);
            return res;
        } else {
            return nullptr;
        }
    }

    // Regular symbol construction
    SymbolExpr* create(const char* begin, const char* end);

    // Curly expansion construction TODO change or remove
    SymbolExpr* create(const char* begin, const char* end, FStr pat, EvalFactory&& dep);

private:
    Mode evaluate;
    Vector<EvalExpr*> exprs;
};

struct TernaryExpr : public EvalExpr {
    TernaryExpr(EvalExpr* cond, EvalExpr* left, EvalExpr* right) : cond(cond), left(left), right(right) {}

    int getId() override { return expr_ternary; }

    EvalExpr* cond;
    EvalExpr* left;
    EvalExpr* right;
};

struct LogicOpExpr : public EvalExpr {
    LogicOpExpr(EvalExpr* lhs, EvalExpr* rhs, char op) : lhs(lhs), rhs(rhs), op(op) {}

    int getId() override { return expr_logic; }

    EvalExpr* lhs;
    EvalExpr* rhs;
    char op;
};

struct CmpOpExpr : public EvalExpr {
    CmpOpExpr(EvalExpr* lhs, EvalExpr* rhs, exprtype_T enumOp, exprcase_T mod = CASE_OPTION) :
        lhs(lhs), rhs(rhs), enumOp(enumOp), mod(mod) {}

    int getId() override { return expr_compare; }

    EvalExpr* lhs;
    EvalExpr* rhs;
    exprtype_T enumOp;
    exprcase_T mod;
};

struct BinOpExpr : public EvalExpr {
    BinOpExpr(EvalExpr* lhs, EvalExpr* rhs, char op) :
        lhs(lhs), rhs(rhs), op(op) {}

    int getId() override { return expr_arith; }

    EvalExpr* lhs;
    EvalExpr* rhs;
    char op;
};

struct PrefixOpExpr : public EvalExpr {
    PrefixOpExpr(EvalExpr* val, char op) : val(val), op(op) {}

    int getId() override { return expr_prefix; }

    EvalExpr* val;
    char op;
};

struct IndexExpr : public EvalExpr {
    IndexExpr(EvalExpr* what, EvalExpr* index) : what(what), index(index) {}

    int getId() override { return expr_index; }

    EvalExpr* what;
    EvalExpr* index;
};

struct IndexRangeExpr : public EvalExpr {
    IndexRangeExpr(EvalExpr* what, EvalExpr* from, EvalExpr* to) :
        what(what), from(from), to(to) {}

    int getId() override { return expr_index2; }

    EvalExpr* what;
    EvalExpr* from;
    EvalExpr* to;
};

struct InvokeExpr : public EvalExpr {
    InvokeExpr(EvalExpr* name, Vector<EvalExpr*> args) : name(name), args(std::move(args)) {}

    int getId() override { return expr_invoke; }

    EvalExpr* name;
    Vector<EvalExpr*> args;
};

// TODO redo
struct LiteralExpr : public EvalExpr {
    LiteralExpr(VarType type, FStr tok) : type(type), lit(std::move(tok)) {}
    LiteralExpr(VarType type, const char* begin, const char* end) : type(type), lit(begin, end) {}
    LiteralExpr(VarType type, const char* lit, int len) : type(type), lit(lit, len) {}

    int getId() override { return expr_literal; }

    VarType type;
    FStr lit;
};

struct ListExpr : public EvalExpr {
    ListExpr(Vector<EvalExpr*> elems) : elems(std::move(elems)) {}

    int getId() override { return expr_list; }

    Vector<EvalExpr*> elems;
};

struct DictExpr : public EvalExpr {
    struct Pair {
        Pair() = default;
        Pair(EvalExpr* key, EvalExpr* value) : key(key), value(value) {}

        EvalExpr* key;
        EvalExpr* value;
    };

    DictExpr(Vector<Pair> entries) : entries(std::move(entries)) {}

    int getId() override { return expr_dict; }

    Vector<Pair> entries;
};

struct NestedExpr : public EvalExpr {
    NestedExpr(EvalExpr* expr) : expr(expr) {}

    int getId() override { return expr_nest; }

    EvalExpr* expr;
};

struct LambdaExpr : public EvalExpr {
    LambdaExpr(Vector<SymbolExpr*> args, EvalExpr* body) : args(std::move(args)), body(body) {}

    int getId() override { return expr_lambda; }

    Vector<SymbolExpr*> args;
    EvalExpr* body;
};

struct SymbolExpr : public EvalExpr {
    SymbolExpr(const char* begin, const char* end) : begin(begin), end(end), pat(begin, end) {}
    SymbolExpr(const char* begin, const char* end, FStr pat, EvalFactory&& f) :
        begin(begin), end(end), pat(std::move(pat)) {
            depSyms = std::move(f);
        }

    int getId() override { return expr_symbol; }

    const char* begin; //< Start of symbol
    const char* end; //< End of symbol
    FStr pat; //< Pattern of curly expansion. Empty for regular symbols
    EvalFactory depSyms; //< Dependent symbols if curly expansion occured
};

inline SymbolExpr* EvalFactory::create(const char*, const char*, FStr, EvalFactory&&) { return nullptr; }

inline SymbolExpr* EvalFactory::create(const char* begin, const char* end) {
    if (evaluate == EVAL_ALL || evaluate == EVAL_SYMBOLS_ONLY) {
        SymbolExpr* res = new SymbolExpr(begin, end);
        exprs.emplace(res);
        return res;
    } else {
        return nullptr;
    }
}

inline SymbolExpr* EvalFactory::findSymbol(const char* p) {
    for (EvalExpr* expr : exprs) {
        if (expr->getId() == EvalExpr::expr_symbol) {
            SymbolExpr* sym = static_cast<SymbolExpr*>(expr);
            if (p >= sym->begin && p < sym->end) {
                return sym;
            }
        }
    }
    return nullptr;
}
