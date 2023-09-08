#pragma once

#include <type_traits>
#include <cassert>

namespace FStrUtil {
    template <typename T>
    int charsNeeded(const T& what) {
        assert(false && "Instantiate template with T");
        return 0;
    }

    template <typename T>
    int charsNeeded(const T* what) {
        assert(false && "Instantiate template with T");
        return 0;
    }

    template <typename T>
    int appendNoCheck(char* dest, const T& what) {
        assert(false && "Instantiate template with T");
        return 0;
    }

    template <typename T>
    int appendNoCheck(char* dest, const T* what) {
        assert(false && "Instantiate template with T");
        return 0;
    }
};
