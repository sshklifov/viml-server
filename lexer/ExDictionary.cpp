#include <ExDictionary.hpp>

#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <cctype>
#include <cstdio>

#include <algorithm>


const ExDictionary& ExDictionary::getSingleton() {
    static ExDictionary res;
    if (!res.isLoaded()) {
        res.loadDict(DICTIONARY_FILE);
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
    std::unique_ptr<char[]> buf(new char[len + 1]);
    s = read(fd, buf.get(), len);
    buf[len] = '\0';
    close(fd);

    if (s != len) {
        return false;
    } else {
        filebuf.swap(buf);
        rebuild();
        return true;
    }
}

void ExDictionary::unload() {
    filebuf.reset(nullptr);
    dictionary.clear();
}

bool ExDictionary::isLoaded() const {
    return !dictionary.empty();
}

int ExDictionary::search(const char* what) const {
    return search(StringView(what));
}

int ExDictionary::lastEqualIdx(const StringView& key, int lo, int hi) const {
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
    StringView loStr = dictionary[hi].req.trunc(key.length());
    if (key == loStr) {
        return hi;
    }
    StringView hiStr = dictionary[lo].req.trunc(key.length());
    if (key == hiStr) {
        return lo;
    }
    return -1;
}

int ExDictionary::firstEqualIdx(const StringView& key, int lo, int hi) const {
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

int ExDictionary::search(const StringView& key) const {
    if (!isLoaded()) {
        assert(false);
        return -1;
    }

    int lo = 0;
    int hi = dictionary.size() - 1;
    for (int guessLen = 1; guessLen <= key.length(); ++guessLen) {
        Entry guessKey;
        guessKey.req = StringView(key.begin, key.begin + guessLen);
        guessKey.opt = StringView(key.begin + guessLen, key.end);
        lo = firstEqualIdx(guessKey.req, lo, hi);
        if (lo == -1) {
            return -1;
        }
        hi = lastEqualIdx(guessKey.req, lo, hi);
        assert(hi != -1);

        Entry loEntry = dictionary[lo];
        if (loEntry.req.length() == guessLen) {
            StringView loStr = loEntry.opt.trunc(guessKey.opt.length());
            if (guessKey.opt == loStr) {
                return lo;
            } else if (lo == hi) {
                return -1;
            } else {
                ++lo;
            }
        }
        // Need to guess more characters
        assert(loEntry.req.length() >= guessKey.req.length());
    }
    return -1;
}

ExDictionary::Entry ExDictionary::getEntry(int dictIdx) const {
    if (!isLoaded()) {
        assert(false);
        return Entry{};
    }
    return dictionary[dictIdx];
}

void ExDictionary::rebuild() {
    dictionary.clear();
    
    const char* begin = filebuf.get();
    while (*begin) {
        dictionary.resize(dictionary.size() + 1);
        Entry& name = dictionary.back();

        const char* end = begin;
        while (isalnum(*end)) {
            ++end;
        }
        name.req.begin = begin;
        name.req.end = end;

        if (*end == '[') {
            begin = end + 1;
            end = begin;
            while (isalnum(*end)) {
                ++end;
            }
            name.opt.begin = begin;
            name.opt.end = end;
            assert(*end == ']');
            ++end;
        } else {
            name.opt.begin = nullptr;
            name.opt.end = nullptr;
        }

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
    for (int i = 1; i < dictionary.size(); ++i) {
        if (!less(dictionary[i - 1], dictionary[i])) {
            sorted = false;
            break;
        }
    }
    if (sorted) {
        return true;
    }

    fputs("Dictionary is not sorted. This is the expected order:\n", stdout);
    std::sort(dictionary.begin(), dictionary.end(), less);
    for (int i = 0; i < dictionary.size(); ++i) {
        printf("%.*s", dictionary[i].req.length(), dictionary[i].req.begin);
        printf("[%.*s]", dictionary[i].opt.length(), dictionary[i].opt.begin);
        fputc('\n', stdout);
    }
    return false;
}
