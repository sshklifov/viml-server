// emhash8::HashMap for C++14/17
// version 1.6.5
// https://github.com/ktprime/emhash/blob/master/hash_table8.hpp
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2021-2023 Huang Yuanbing & bailuzhou AT 163.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE

#pragma once

#include "RobinHood.hpp"

#include <type_traits>
#include <utility>
#include <cassert>

#undef  EMH_NEW
#undef  EMH_EMPTY

// likely/unlikely
#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
#    define EMH_LIKELY(condition)   __builtin_expect(condition, 1)
#    define EMH_UNLIKELY(condition) __builtin_expect(condition, 0)
#else
#    define EMH_LIKELY(condition)   condition
#    define EMH_UNLIKELY(condition) condition
#endif

#define EMH_EMPTY(n) (0 > (int)(_index[n].next))
#define EMH_EQHASH(n, key_hash) (((uint32_t)(key_hash) & ~_mask) == (_index[n].slot & ~_mask))
#define EMH_NEW(key, len, args, bucket, key_hash) \
    new(_pairs + _num_filled) Entry(Key(key, len), std::forward<Types>(args)...); \
    _etail = bucket; \
    _index[bucket] = {bucket, _num_filled++ | ((uint32_t)(key_hash) & ~_mask)}

#define EMH_INDEX_INACTIVE (~0u)
#define EMH_INDEX_EAD 2

namespace emhash8 {

template <typename T>
class StringMap {
    struct Key {
        const char* str;
        int len;

        Key(const char* s, int n) : str(s), len(n) {}

        bool operator==(const Key& rhs) const {
            return len == rhs.len && strncmp(str, rhs.str, len) == 0;
        }
    };

    struct Entry {
        Key key;
        T val;

        template <typename... Types>
        Entry(const Key& key, Types&&... args) :
            key(key), val(std::forward<Types>(args)...) {}
    };

    struct Index {
        uint32_t next;
        uint32_t slot;
    };

public:
    constexpr static float EMH_DEFAULT_LOAD_FACTOR = 0.80f;
    constexpr static float EMH_MIN_LOAD_FACTOR = 0.25f;

    StringMap(uint32_t bucket = 2, float mlf = EMH_DEFAULT_LOAD_FACTOR) {
        init(bucket, mlf);
    }

    StringMap(const StringMap& rhs) {
        if (rhs.load_factor() > EMH_MIN_LOAD_FACTOR) {
            _pairs = realloc_bucket((uint32_t)(rhs._num_buckets * rhs.max_load_factor()) + 4);
            _index = realloc_index(rhs._num_buckets);
            clone(rhs);
        } else {
            init(rhs._num_filled + 2, EMH_DEFAULT_LOAD_FACTOR);
            for (Entry* it = rhs.begin(); it != rhs.end(); ++it) {
                emplace_unique(it->key, it->val);
            }
        }
    }

    StringMap(StringMap&& rhs) noexcept {
        init(0);
        *this = std::move(rhs);
    }

    ~StringMap() noexcept {
        clearkv();
        free(_pairs);
        free(_index);
    }

    StringMap& operator=(const StringMap& rhs) {
        if (this == &rhs) {
            return *this;
        }

        if (rhs.load_factor() < EMH_MIN_LOAD_FACTOR) {
            clear(); free(_pairs); _pairs = nullptr;
            rehash(rhs._num_filled + 2);
            for (Entry* it = rhs.begin(); it != rhs.end(); ++it) {
                emplace_unique(it->key, it->val);
            }
            return *this;
        }

        clearkv();

        if (_num_buckets != rhs._num_buckets) {
            free(_pairs); free(_index);
            realloc_index(rhs._num_buckets);
            realloc_bucket((uint32_t)(rhs._num_buckets * rhs.max_load_factor()) + 4);
        }

        clone(rhs);
        return *this;
    }

    StringMap& operator=(StringMap&& rhs) noexcept {
        if (this != &rhs) {
            swap(rhs);
            rhs.clear();
        }
        return *this;
    }

    void swap(StringMap& rhs) {
        std::swap(_pairs, rhs._pairs);
        std::swap(_index, rhs._index);
        std::swap(_num_buckets, rhs._num_buckets);
        std::swap(_num_filled, rhs._num_filled);
        std::swap(_mask, rhs._mask);
        std::swap(_mlf, rhs._mlf);
        std::swap(_last, rhs._last);
        std::swap(_etail, rhs._etail);
    }

    inline Entry* begin() { return _pairs; }
    inline Entry* end() { return _pairs + _num_filled; }

    inline uint32_t count() const { return _num_filled; }
    inline bool empty() const { return _num_filled == 0; }

    inline T find(const char* key, int len) noexcept {
        int idx = find_filled_slot(Key(key, len));
        assert(idx < count());
        return _pairs[idx].val;
    }

    template<typename... Types>
    void emplace_unique(const char* key, int len, Types&&... args) {
        check_expand_need();
        const uint64_t key_hash = hash_bytes(key, len);
        uint32_t bucket = find_unique_bucket(key_hash);
        EMH_NEW(key, len, args, bucket, key_hash);
    }

    template<typename... Types>
    Entry* emplace(const char* key, int len, Types&&... args) {
        check_expand_need();

        const uint64_t key_hash = hash_bytes(key, len);
        const uint32_t bucket = find_or_allocate(Key(key, len), key_hash);
        const bool bempty = EMH_EMPTY(bucket);
        if (bempty) {
            EMH_NEW({key, len}, args, bucket, key_hash);
        }

        const uint32_t slot = _index[bucket].slot & _mask;
        return _pairs + slot;
    }

    /// Erase an element from the hash table.
    bool erase(const char* key, int len) noexcept {
        const uint64_t key_hash = hash_bytes(key, len);
        const uint32_t sbucket = find_filled_bucket(Key(key, len), key_hash);
        if (sbucket == EMH_INDEX_INACTIVE) {
            return 0;
        }

        const uint32_t main_bucket = key_hash & _mask;
        erase_slot(sbucket, main_bucket);
        return 1;
    }

    void erase(const Entry* it) noexcept {
        const uint32_t slot = it - _pairs;
        uint32_t main_bucket;
        const uint32_t sbucket = find_slot_bucket(slot, main_bucket);
        erase_slot(sbucket, main_bucket);
    }

    /// Remove all elements, keeping full capacity.
    void clear() noexcept {
        clearkv();

        if (_num_filled > 0) {
            memset((char*)_index, EMH_INDEX_INACTIVE, sizeof(_index[0]) * _num_buckets);
        }

        _last = _num_filled = 0;
        _etail = EMH_INDEX_INACTIVE;
    }

private:
    inline uint32_t bucket_count() const { return _num_buckets; }

    /// Returns average number of elements per bucket.
    inline float load_factor() const { return static_cast<float>(_num_filled) / (_mask + 1); }

    void max_load_factor(float mlf) {
        if (mlf < 0.992 && mlf > EMH_MIN_LOAD_FACTOR) {
            _mlf = (uint32_t)((1 << 27) / mlf);
            if (_num_buckets > 0) rehash(_num_buckets);
        }
    }

    inline constexpr float max_load_factor() const { return (1 << 27) / (float)_mlf; }
    inline constexpr uint32_t max_bucket_count() const { return (1ull << (sizeof(uint32_t) * 8 - 1)); }

    void init(uint32_t bucket, float mlf = EMH_DEFAULT_LOAD_FACTOR) {
        _pairs = nullptr;
        _index = nullptr;
        _mask  = _num_buckets = 0;
        _num_filled = 0;
        _mlf = (uint32_t)((1 << 27) / EMH_DEFAULT_LOAD_FACTOR);
        max_load_factor(mlf);
        rehash(bucket);
    }

    void clone(const StringMap& rhs) {
        _num_buckets = rhs._num_buckets;
        _num_filled  = rhs._num_filled;
        _mlf         = rhs._mlf;
        _last        = rhs._last;
        _mask        = rhs._mask;
        _etail       = rhs._etail;

        Entry* opairs  = rhs._pairs;
        memcpy((char*)_index, (char*)rhs._index, (_num_buckets + EMH_INDEX_EAD) * sizeof(Index));
        memcpy((char*)_pairs, (char*)opairs, _num_filled * sizeof(Entry));
    }

    void clearkv() {
        if (!std::is_trivially_destructible<T>()) {
            while (_num_filled --)
                _pairs[_num_filled].~Entry();
        }
    }

    void realloc_bucket(uint32_t num_buckets) {
        _pairs = (Entry*)realloc(_pairs, (uint64_t)num_buckets * sizeof(Entry));
    }

    void realloc_index(uint32_t num_buckets) {
        _index = (Index*)realloc(_index, (uint64_t)(EMH_INDEX_EAD + num_buckets) * sizeof(Index));
    }

    void rebuild(uint32_t num_buckets) noexcept {
        realloc_bucket((uint32_t)(num_buckets * max_load_factor()) + 4);
        realloc_index(num_buckets);

        memset(_index, EMH_INDEX_INACTIVE, sizeof(_index[0]) * num_buckets);
        memset(_index + num_buckets, 0, sizeof(_index[0]) * EMH_INDEX_EAD);
    }

    void rehash(uint64_t required_buckets) {
        if (required_buckets < _num_filled) {
            return;
        }

        assert(required_buckets < max_bucket_count());
        uint64_t num_buckets = _num_filled > (1u << 16) ? (1u << 16) : 4u;
        while (num_buckets < required_buckets) { num_buckets *= 2; }

        _last = _mask / 4;
        _mask        = num_buckets - 1;
        _num_buckets = num_buckets;

        rebuild(num_buckets);

        _etail = EMH_INDEX_INACTIVE;
        for (uint32_t slot = 0; slot < _num_filled; ++slot) {
            const Key& key = _pairs[slot].key;
            const size_t key_hash = hash_bytes(key.str, key.len);
            const uint32_t bucket = find_unique_bucket(key_hash);
            _index[bucket] = { bucket, slot | ((uint32_t)(key_hash) & ~_mask) };
        }
    }

    // Can we fit another element?
    inline bool check_expand_need() {
        uint64_t num_elems = _num_filled;
        const uint32_t required_buckets = num_elems * _mlf >> 27;
        if (EMH_LIKELY(required_buckets < _mask)) {
            return false;
        }

        rehash(required_buckets + 2);
        return true;
    }

    uint32_t slot_to_bucket(uint32_t slot) const noexcept {
        uint32_t main_bucket;
        return find_slot_bucket(slot, main_bucket);
    }

    // Very slow
    void erase_slot(const uint32_t sbucket, const uint32_t main_bucket) noexcept {
        const uint32_t slot = _index[sbucket].slot & _mask;
        const uint32_t ebucket = erase_bucket(sbucket, main_bucket);
        const uint32_t last_slot = --_num_filled;
        if (EMH_LIKELY(slot != last_slot)) {
            const uint32_t last_bucket =
                (_etail == EMH_INDEX_INACTIVE || ebucket == _etail) ? slot_to_bucket(last_slot) : _etail;

            _pairs[slot] = std::move(_pairs[last_slot]);
            _index[last_bucket].slot = slot | (_index[last_bucket].slot & ~_mask);
        }

        if (!std::is_trivially_destructible<T>::value) {
            _pairs[last_slot].~Entry();
        }

        _etail = EMH_INDEX_INACTIVE;
        _index[ebucket] = {EMH_INDEX_INACTIVE, 0};
    }

    uint32_t erase_bucket(uint32_t bucket, uint32_t main_bucket) noexcept {
        const uint32_t next_bucket = _index[bucket].next;
        if (bucket == main_bucket) {
            if (main_bucket != next_bucket) {
                const uint32_t nbucket = _index[next_bucket].next;
                _index[main_bucket] = {
                    (nbucket == next_bucket) ? main_bucket : nbucket,
                    _index[next_bucket].slot
                };
            }
            return next_bucket;
        }

        const uint32_t prev_bucket = find_prev_bucket(main_bucket, bucket);
        _index[prev_bucket].next = (bucket == next_bucket) ? prev_bucket : next_bucket;
        return bucket;
    }

    // Find the slot with this key, or return bucket size
    uint32_t find_slot_bucket(uint32_t slot, uint32_t& main_bucket) const {
        const uint64_t key_hash = hash_bytes(_pairs[slot].key.str, _pairs[slot].key.len);
        const uint32_t bucket = main_bucket = uint32_t(key_hash & _mask);
        if (slot == (_index[bucket].slot & _mask)) {
            return bucket;
        }

        uint32_t next_bucket = _index[bucket].next;
        while (true) {
            if (EMH_LIKELY(slot == (_index[next_bucket].slot & _mask))) {
                return next_bucket;
            }
            next_bucket = _index[next_bucket].next;
        }

        return EMH_INDEX_INACTIVE;
    }

    // Find the slot with this key, or return bucket size
    uint32_t find_filled_bucket(const Key& key, uint64_t key_hash) const noexcept {
        const uint32_t bucket = key_hash & _mask;
        uint32_t next_bucket  = _index[bucket].next;
        if (EMH_UNLIKELY((int)next_bucket < 0)) {
            return EMH_INDEX_INACTIVE;
        }

        if (EMH_EQHASH(bucket, key_hash)) {
            const uint32_t slot = _index[bucket].slot & _mask;
            if (EMH_LIKELY(key == _pairs[slot].first)) {
                return bucket;
            }
        }
        if (next_bucket == bucket) {
            return EMH_INDEX_INACTIVE;
        }

        while (true) {
            if (EMH_EQHASH(next_bucket, key_hash)) {
                const uint32_t slot = _index[next_bucket].slot & _mask;
                if (EMH_LIKELY(key == _pairs[slot].first)) {
                    return next_bucket;
                }
            }

            const uint32_t nbucket = _index[next_bucket].next;
            if (nbucket == next_bucket) {
                return EMH_INDEX_INACTIVE;
            }
            next_bucket = nbucket;
        }

        return EMH_INDEX_INACTIVE;
    }

    // Find the slot with this key, or return bucket size
    uint32_t find_filled_slot(const Key& key) const noexcept {
        const uint64_t key_hash = hash_bytes(key.str, key.len);
        const uint32_t bucket = uint32_t(key_hash & _mask);
        uint32_t next_bucket = _index[bucket].next;
        if ((int)next_bucket < 0)
            return _num_filled;

        if (EMH_EQHASH(bucket, key_hash)) {
            const uint32_t slot = _index[bucket].slot & _mask;
            if (EMH_LIKELY(key == _pairs[slot].key)) {
                return slot;
            }
        }
        if (next_bucket == bucket) {
            return _num_filled;
        }

        while (true) {
            if (EMH_EQHASH(next_bucket, key_hash)) {
                const uint32_t slot = _index[next_bucket].slot & _mask;
                if (EMH_LIKELY(key == _pairs[slot].key)) {
                    return slot;
                }
            }

            const uint32_t nbucket = _index[next_bucket].next;
            if (nbucket == next_bucket){
                return _num_filled;
            }
            next_bucket = nbucket;
        }

        return _num_filled;
    }

    //kick out bucket and find empty to occpuy
    //it will break the orgin link and relnik again.
    //before: main_bucket-->prev_bucket --> bucket   --> next_bucket
    //atfer : main_bucket-->prev_bucket --> (removed)--> new_bucket--> next_bucket
    uint32_t kickout_bucket(const uint32_t kmain, const uint32_t bucket) noexcept {
        const uint32_t next_bucket = _index[bucket].next;
        const uint32_t new_bucket  = find_empty_bucket(next_bucket);
        const uint32_t prev_bucket = find_prev_bucket(kmain, bucket);

        const uint32_t last = next_bucket == bucket ? new_bucket : next_bucket;
        _index[new_bucket] = {last, _index[bucket].slot};

        _index[prev_bucket].next = new_bucket;
        _index[bucket].next = EMH_INDEX_INACTIVE;

        return bucket;
    }

    /*
     ** inserts a new key into a hash table; first, check whether key's main
     ** bucket/position is free. If not, check whether colliding node/bucket is in its main
     ** position or not: if it is not, move colliding bucket to an empty place and
     ** put new key in its main position; otherwise (colliding bucket is in its main
     ** position), new key goes to an empty position.
     */
    uint32_t find_or_allocate(const Key& key, uint64_t key_hash) noexcept {
        const uint32_t bucket = uint32_t(key_hash & _mask);
        uint32_t next_bucket = _index[bucket].next;
        if ((int)next_bucket < 0) {
            return bucket;
        }

        const uint32_t slot = _index[bucket].slot & _mask;
        if (EMH_EQHASH(bucket, key_hash)) {
            if (EMH_LIKELY(key == _pairs[slot].key)) {
                return bucket;
            }
        }

        //check current bucket_key is in main bucket or not
        const uint32_t kmain = hash_bucket(_pairs[slot].key);
        if (kmain != bucket) {
            return kickout_bucket(kmain, bucket);
        }
        else if (next_bucket == bucket) {
            return _index[next_bucket].next = find_empty_bucket(next_bucket);
        }

        //find next linked bucket and check key
        while (true) {
            const uint32_t eslot = _index[next_bucket].slot & _mask;
            if (EMH_EQHASH(next_bucket, key_hash)) {
                if (EMH_LIKELY(key == _pairs[eslot].key)) {
                    return next_bucket;
                }
            }

            const uint32_t nbucket = _index[next_bucket].next;
            if (nbucket == next_bucket) {
                break;
            }
            next_bucket = nbucket;
        }

        //find a empty and link it to tail
        const uint32_t new_bucket = find_empty_bucket(next_bucket);
        return _index[next_bucket].next = new_bucket;
    }

    uint32_t find_unique_bucket(uint64_t key_hash) noexcept {
        const uint32_t bucket = uint32_t(key_hash & _mask);
        uint32_t next_bucket = _index[bucket].next;
        if ((int)next_bucket < 0) {
            return bucket;
        }

        //check current bucket_key is in main bucket or not
        const uint32_t kmain = hash_main(bucket);
        if (EMH_UNLIKELY(kmain != bucket)) {
            return kickout_bucket(kmain, bucket);
        }
        else if (EMH_UNLIKELY(next_bucket != bucket)) {
            next_bucket = find_last_bucket(next_bucket);
        }

        return _index[next_bucket].next = find_empty_bucket(next_bucket);
    }

    /***
      Different probing techniques usually provide a trade-off between memory locality and avoidance of clustering.
      Since Robin Hood hashing is relatively resilient to clustering (both primary and secondary), linear probing is the most cache friendly alternativeis typically used.

      It's the core algorithm of this hash map with highly optimization/benchmark.
      normaly linear probing is inefficient with high load factor, it use a new 3-way linear
      probing strategy to search empty slot. from benchmark even the load factor > 0.9, it's more 2-3 timer fast than
      one-way search strategy.

      1. linear or quadratic probing a few cache line for less cache miss from input slot "bucket_from".
      2. the first  search  slot from member variant "_last", init with 0
      3. the second search slot from calculated pos "(_num_filled + _last) & _mask", it's like a rand value
      */
    // key is not in this mavalue. Find a place to put it.
    uint32_t find_empty_bucket(uint32_t bucket_from) noexcept {
        uint32_t bucket = bucket_from;
        if (EMH_EMPTY(++bucket) || EMH_EMPTY(++bucket)) {
            return bucket;
        }

        constexpr uint32_t quadratic_probe_length = 6u;
        for (uint32_t offset = 4u, step = 3u; step < quadratic_probe_length; ) {
            bucket = (bucket_from + offset) & _mask;
            if (EMH_EMPTY(bucket) || EMH_EMPTY(++bucket)) {
                return bucket;
            }
            offset += step++;
        }

        for (;;) {
            if (EMH_EMPTY(++_last)) {
                return _last;
            }

            _last &= _mask;
            uint32_t medium = (_num_buckets / 2 + _last) & _mask;
            if (EMH_EMPTY(medium)) {
                return medium;
            }
        }

        return 0;
    }

    uint32_t find_last_bucket(uint32_t main_bucket) const {
        uint32_t next_bucket = _index[main_bucket].next;
        if (next_bucket == main_bucket) {
            return main_bucket;
        }

        while (true) {
            const uint32_t nbucket = _index[next_bucket].next;
            if (nbucket == next_bucket) {
                return next_bucket;
            }
            next_bucket = nbucket;
        }
    }

    uint32_t find_prev_bucket(const uint32_t main_bucket, const uint32_t bucket) const {
        uint32_t next_bucket = _index[main_bucket].next;
        if (next_bucket == bucket) {
            return main_bucket;
        }

        while (true) {
            const uint32_t nbucket = _index[next_bucket].next;
            if (nbucket == bucket) {
                return next_bucket;
            }
            next_bucket = nbucket;
        }
    }

    inline uint32_t hash_bucket(const Key& key) const noexcept {
        return (uint32_t)hash_bytes(key.str, key.len) & _mask;
    }

    inline uint32_t hash_main(const uint32_t bucket) const noexcept {
        const uint32_t slot = _index[bucket].slot & _mask;
        return (uint32_t)hash_bytes(_pairs[slot].key.str, _pairs[slot].key.len) & _mask;
    }

private:
    Index* _index;
    Entry* _pairs;

    uint32_t  _mlf;
    uint32_t _mask;
    uint32_t _num_buckets;
    uint32_t _num_filled;
    uint32_t _last;
    uint32_t _etail;
};

} // namespace emhash
