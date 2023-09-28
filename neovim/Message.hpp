#pragma once

#include <FStr.hpp>

struct msg {
    template <typename... Types>
    msg(const char* where, const Types&... args) {
        ppos = where;
        message.appendf(args...);
    }

    const char* ppos;
    FStr message;
};
