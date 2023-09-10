#include <ExDictionary.hpp>
#include <ExConstants.hpp>

#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <cctype>
#include <cstdio>

#include <algorithm>

ExDictionary::~ExDictionary() {
    delete[] filebuf;
}

const ExDictionary& ExDictionary::getSingleton() {
    static ExDictionary res;
    if (!res.isLoaded()) {
        res.loadDict(DICTIONARY_FILE);
        assert(debugCheckExConstants());
    }
    return res;
}

bool ExDictionary::loadDict(const char* filepath) {
    if (isLoaded()) {
        assert(false);
        return false;
    }

    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        return false;
    }

    struct stat statbuf;
    ssize_t s = fstat(fd, &statbuf);
    if (s != 0) {
        close(fd);
        return false;
    }

    size_t len =  statbuf.st_size;
    char* buf = new char[len + 1];
    s = read(fd, buf, len);
    buf[len] = '\0';
    close(fd);

    if (s != len) {
        delete[] buf;
        return false;
    } else {
        delete[] filebuf;
        filebuf = buf;
        rebuild();
        return true;
    }
}

void ExDictionary::unload() {
    delete[] filebuf;
    filebuf = nullptr;
    dictionary.clear();
}

bool ExDictionary::isLoaded() const {
    return !dictionary.empty();
}

int ExDictionary::lastReqMatch(const StringView& key, int lo, int hi) const {
    while (lo + 1 < hi) {
        int mid = (lo + hi) / 2;
        StringView midStr = dictionary[mid].req.trunc(key.length());
        int c = key.cmp(midStr);
        if (c < 0) {
            hi = mid - 1;
        } else if (c > 0) {
            lo = mid + 1;
        } else {
            lo = mid;
        }
    }
    StringView hiStr = dictionary[hi].req.trunc(key.length());
    if (key == hiStr) {
        return hi;
    }
    StringView loStr = dictionary[lo].req.trunc(key.length());
    if (key == loStr) {
        return lo;
    }
    return -1;
}

int ExDictionary::firstReqMatch(const StringView& key, int lo, int hi) const {
    while (lo + 1 < hi) {
        int mid = (lo + hi) / 2;
        StringView midStr = dictionary[mid].req.trunc(key.length());
        int c = key.cmp(midStr);
        if (c < 0) {
            hi = mid - 1;
        } else if (c > 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    StringView loStr = dictionary[lo].req.trunc(key.length());
    if (key == loStr) {
        return lo;
    }
    StringView hiStr = dictionary[hi].req.trunc(key.length());
    if (key == hiStr) {
        return hi;
    }
    return -1;
}

int ExDictionary::search(const FStr& key) const {
    return search(StringView(key.str(), key.length()));
}

int ExDictionary::search(const StringView& key) const {
    int maxMatched = 0;
    int res = partialSearch(key, maxMatched);
    if (maxMatched == key.length()) {
        return res;
    } else {
        return -1;
    }
}

int ExDictionary::partialSearch(const char* key, int& maxMatched) const {
    return partialSearch(StringView(key), maxMatched);
}

int ExDictionary::partialSearch(const StringView& key, int& maxMatched) const {
    if (!isLoaded()) {
        assert(false);
        return -1;
    }

    maxMatched = 0;
    int resultIdx = -1;

    int lo = 0;
    int hi = dictionary.count() - 1;
    for (int guessLen = 1; guessLen <= key.length(); ++guessLen) {
        Entry guessKey(key, guessLen);
        lo = firstReqMatch(guessKey.req, lo, hi);
        if (lo < 0) {
            return resultIdx;
        }
        hi = lastReqMatch(guessKey.req, lo, hi);
        assert(lo <= hi);

        const Entry& resEntry = dictionary[lo];
        assert(resEntry.req.length() >= guessLen);
        if (resEntry.req.length() == guessLen) {
            StringView resOpt = resEntry.opt;
            StringView keyOpt = guessKey.opt;
            int numMatched = guessLen;
            while (!resOpt.empty() && !keyOpt.empty()) {
                if (resOpt.left() != keyOpt.left()) {
                    break;
                }
                ++numMatched;
                ++resOpt.begin;
                ++keyOpt.begin;
            }
            if (numMatched > maxMatched) {
                maxMatched = numMatched;
                resultIdx = lo;
            }
            ++lo;
            if (lo > hi) {
                return resultIdx;
            }
        }
    }
    return resultIdx;
}

ExDictionary::Entry ExDictionary::getEntry(int dictIdx) const {
    assert(dictIdx >= 0);
    if (!isLoaded()) {
        assert(false);
        return Entry{};
    }
    return dictionary[dictIdx];
}

void ExDictionary::rebuild() {
    dictionary.clear();
    
    const char* begin = filebuf;
    while (*begin) {
        StringView req, opt;

        const char* end = begin;
        while (*end != '[') {
            ++end;
        }
        req.begin = begin;
        req.end = end;

        begin = end + 1;
        end = begin;
        while (*end != ']') {
            ++end;
        }
        opt.begin = begin;
        opt.end = end;

        dictionary.emplace(req, opt);

        ++end;
        assert(*end == '\n');
        begin = end + 1;
    }

    assert(debugCheckSorted());
}

static int less(const ExDictionary::Entry& lhs, const ExDictionary::Entry& rhs) {
    return lhs.req < rhs.req;
}

bool ExDictionary::debugCheckSorted() {
    bool sorted = true;
    for (int i = 1; i < dictionary.count(); ++i) {
        if (!less(dictionary[i - 1], dictionary[i])) {
            sorted = false;
            break;
        }
    }
    if (sorted) {
        return true;
    }

    fputs("Dictionary is not sorted. This is the expected order:\n", stdout);
    std::sort(&dictionary.first(), &dictionary.last(), less);
    for (int i = 0; i < dictionary.count(); ++i) {
        printf("%.*s", dictionary[i].req.length(), dictionary[i].req.begin);
        printf("[%.*s]", dictionary[i].opt.length(), dictionary[i].opt.begin);
        fputc('\n', stdout);
    }
    return false;
}
