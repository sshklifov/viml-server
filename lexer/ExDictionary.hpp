#pragma once

#include <StringView.hpp>

#include <vector>
#include <memory>

struct ExDictionary {
    struct Entry {
        StringView req;
        StringView opt;
    };

    bool loadDict(const char* filepath);
    void unload();
    bool isLoaded() const;

    int search(const char* cmd) const;
    int search(const StringView& cmd) const;

    Entry getEntry(int dictIdx) const;

private:
    int firstEqualIdx(const StringView& key, int lo, int hi) const;
    int lastEqualIdx(const StringView& key, int lo, int hi) const;

    void rebuild();
    bool debugCheckSorted();
   
    std::vector<Entry> dictionary;
    std::unique_ptr<char[]> filebuf;
};
