#pragma once

#include "NeovimBase.hpp"

struct vimvar {
    VarType type;  ///< Type of variable
    unsigned char vv_flags;  ///< Flags: #VV_COMPAT, #VV_RO, #VV_RO_SBX.
};

vimvar get_vimvar(const char* arg, int len);
