#pragma once

#include "NeovimBase.hpp"

/// Type of option "opt". VAR_UKNOWN is not a valid option
VarType get_option_type(const char *opt, int len);
