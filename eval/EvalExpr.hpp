#pragma once

#include <FStr.hpp>
#include "EvalBase.hpp"

struct EvalExpr : public EvalBase {
    virtual void appendStr(FStr& res) = 0;
};
