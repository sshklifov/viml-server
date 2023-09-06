#pragma once

#include "EvalBase.hpp"

#include <vector>

struct EvalFactory {
    EvalFactory() = default;
    EvalFactory(const EvalFactory&) = delete;
    EvalFactory(EvalFactory&&) = delete;

    ~EvalFactory() {
        for (EvalBase* obj : objs) {
            delete obj;
        }
    }

    template <typename T, typename ... Args>
    T* create(Args&&... args) {
        T* res = new T(std::forward<Args>(args)...);
        objs.push_back(res);
        return res;
    }

private:
    std::vector<EvalBase*> objs;
};
