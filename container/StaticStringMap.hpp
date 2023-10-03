#pragma once

#include "Vector.hpp"
#include "FStr.hpp"

#include <cstring>

template <typename Value>
struct StaticStringMap {
    struct Entry {
        const char* key;
        Value val;

        template <typename... Types>
        Entry(const char* key, Types&&... args)
        : key(key), val(std::forward<Types>(args)...) {}

        bool operator<(const Entry& rhs) {
            return strcmp(key, rhs.key) < 0;
        }
    };

    StaticStringMap() {
        quick = new int[128];
        memset(quick, 0, 128 * sizeof(int));
    }

    ~StaticStringMap() {
        delete[] quick;
    }

    template <typename... Types>
    int emplace(const char* key, Types&&... args) {
        assert(map.empty() || strcmp(map.last().key, key) < 0);
        if (!map.empty()) {
            int prevIdx = *map.last().key;
            int currIdx = *key;
            for (int i = prevIdx + 1; i <= currIdx + 1; ++i) {
                quick[i] = map.count();
            }
        }
        map.emplace(key, std::forward<Types>(args)...);
        return map.count() - 1;
    }

    int find(const char* key, int n) const {
        assert(!map.empty());
        int qidx = *key;
        int lo = quick[qidx];
        int hi = quick[qidx + 1];

        while (hi - lo > 1) {
            int mid = (lo + hi) / 2;
            int cmp = strncmp(key, map[mid].key, n);
            if (cmp < 0) {
                hi = mid;
            } else {
                lo = mid;
            }
        }
        if (strncmp(key, map[lo].key, n) == 0) {
            return lo;
        } else {
            return -1;
        }
    }

    int find(const char* key) const {
        return find(key, strlen(key));
    }

    int find(const FStr& f) const {
        return find(f.str(), f.length());
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

    // For the brave

    const Value& operator[](const char* key) const {
        int pos = find(key);
        assert(pos >= 0);
        return map[pos].val;
    }

    Value& operator[](const char* key) {
        int pos = find(key);
        assert(pos >= 0);
        return map[pos].val;
    }

private:
    Vector<Entry> map;
    int* quick;
};


