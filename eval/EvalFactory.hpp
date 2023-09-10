#pragma once

#include "EvalBase.hpp"
#include <Vector.hpp>

struct EvalFactory {
    EvalFactory() = default;
    EvalFactory(const EvalFactory&) = delete;
    EvalFactory(EvalFactory&&) = delete;

    ~EvalFactory() {
        clear();
    }

    void clear() {
        for (EvalBase* obj : objs) {
            delete obj;
        }
        objs.clear();
    }

    template <typename T, typename ... Args>
    T* create(Args&&... args) {
        T* res = new T(std::forward<Args>(args)...);
        objs.emplace(res);
        return res;
    }

private:
    Vector<EvalBase*> objs;
};
