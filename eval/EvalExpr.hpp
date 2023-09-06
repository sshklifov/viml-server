#pragma once

#include "EvalBase.hpp"

#include <string>

struct EvalExpr : public EvalBase {
    virtual std::string toString() = 0;
};
