#pragma once

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
