#pragma once

#include "Vector.hpp"
#include "FStr.hpp"

template <typename Value>
struct SmallStringMap {
    struct Entry {
        const char* key;
        Value val;

        template <typename... Types>
        Entry(const char* key, Types&&... args)
        : key(key), val(std::forward<Types>(args)...) {}
    };

    template <typename... Types>
    int emplace(const char* key, Types&&... args) {
        map.emplace(key, std::forward<Types>(args)...);
        return map.count() - 1;
    }

    bool erase(const char* key) {
        int pos = find(key);
        if (pos < 0) {
            return false;
        }
        erase(pos);
        return true;
    }

    void erase(int pos) {
        map.remove(pos);
    }

    int find(const char* key) const {
        for (int i = 0; i < map.count(); ++i) {
            if (strcmp(key, map[i].key) == 0) {
                return i;
            }
        }
        return -1;
    }

    int find(const FStr& f) const {
        return find(f.str());
    }

    int count() const {
        return map.count();
    }

    Value& operator[](int pos) {
        return map[pos].val;
    }

    const Value& operator[](int pos) const {
        return map[pos].val;
    }

private:
    Vector<Entry> map;
};


