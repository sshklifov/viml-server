#pragma once

#include "EvalBase.hpp"

#include <string>
#include <cassert> // TODO

struct EvalCommand : public EvalBase {
    virtual std::string toString() { assert(false && "TODO"); }
};
