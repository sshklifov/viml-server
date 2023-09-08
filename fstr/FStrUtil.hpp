#pragma once

#include <type_traits>
#include <cassert>

namespace FStrUtil {
    template <typename T, typename std::enable_if<std::is_scalar<T>::value, bool>::type = true>
    int charsNeeded(T what) {
        assert(false && "Instantiate template with T");
        return 0;
    }
    template <typename T, typename std::enable_if<!std::is_scalar<T>::value, bool>::type = true>
    int charsNeeded(const T& what) {
        assert(false && "Instantiate template with T");
        return 0;
    }

    template <typename T, typename std::enable_if<std::is_scalar<T>::value, bool>::type = true>
    int appendNoCheck(char* dest, T what) {
        assert(false && "Instantiate template with T");
        return 0;
    }

    template <typename T, typename std::enable_if<!std::is_scalar<T>::value, bool>::type = true>
    int appendNoCheck(char* dest, const T& what) {
        assert(false && "Instantiate template with T");
        return 0;
    }
};
