#pragma once

#include <FStr.hpp>
#include <StringView.hpp>
#include <Vector.hpp>

#include <memory>

struct ExDictionary {
    struct Entry {
        Entry() = default;
        Entry(const StringView& req, const StringView& opt) : req(req), opt(opt) {}
        Entry(const StringView& cmd, int splitIdx) : req(cmd.begin, cmd.begin + splitIdx), opt(cmd.begin + splitIdx, cmd.end) {}

        StringView req;
        StringView opt;
    };

    ~ExDictionary();

    bool loadDict(const char* filepath);
    void unload();
    bool isLoaded() const;

    int search(const FStr& cmd) const;
    int search(const StringView& cmd) const;

    int partialSearch(const char* cmd, int& numMatched) const;
    int partialSearch(const StringView& cmd, int& numMatched) const;

    Entry getEntry(int dictIdx) const;

	static const ExDictionary& getSingleton();

private:
    int firstReqMatch(const StringView& key, int lo, int hi) const;
    int lastReqMatch(const StringView& key, int lo, int hi) const;

    void rebuild();
    bool debugCheckSorted();
   
    Vector<Entry> dictionary;
    char* filebuf;
};
