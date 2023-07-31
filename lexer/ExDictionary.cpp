#include <ExDictionary.hpp>

#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <algorithm>

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
    cmds.clear();
}

bool ExDictionary::isLoaded() const {
    return !cmds.empty();
}

int ExDictionary::search(const char* what) const {
    return search(StringView(what));
}

int ExDictionary::lastEqualIdx(const StringView& key, int lo, int hi) const {
    while (lo + 1 < hi) {
        int mid = (lo + hi) / 2;
        StringView dictMid = cmds[mid].req.substring(key.length());
        int c = key.cmp(dictMid);
        if (c < 0) {
            hi = mid - 1;
        } else if (c > 0) {
            lo = mid + 1;
        } else {
            lo = mid;
        }
    }
    StringView dictHi = cmds[hi].req.substring(key.length());
    if (dictHi == key) {
        return hi;
    }
    StringView dictLo = cmds[lo].req.substring(key.length());
    if (dictLo == key) {
        return lo;
    }
    return -1;
}

int ExDictionary::firstEqualIdx(const StringView& key, int lo, int hi) const {
    while (lo + 1 < hi) {
        int mid = (lo + hi) / 2;
        StringView dictMid = cmds[mid].req.substring(key.length());
        int c = key.cmp(dictMid);
        if (c < 0) {
            hi = mid - 1;
        } else if (c > 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    StringView dictLo = cmds[lo].req.substring(key.length());
    if (dictLo == key) {
        return lo;
    }
    StringView dictHi = cmds[hi].req.substring(key.length());
    if (dictHi == key) {
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
    int hi = cmds.size() - 1;

    for (int guessLen = 1; guessLen <= key.length(); ++guessLen) {
        Name guessKey;
        key.split(guessLen, guessKey.req, guessKey.opt);
        int newLo = firstEqualIdx(guessKey.req, lo, hi);
        if (newLo == -1) {
            return -1;
        }
        int newHi = lastEqualIdx(guessKey.req, newLo, hi);
        assert(newHi != -1);

        Name dictMatch = cmds[newLo];
        if (dictMatch.req.length() == guessLen) {
            dictMatch.opt.shrink(guessKey.opt.length());
            if (dictMatch.opt == guessKey.opt) {
                return newLo;
            } else {
                if (newLo == newHi) {
                    return -1;
                } else {
                    ++newLo;
                }
            }
        }
        // Need to guess more characters
        assert(dictMatch.req.length() >= guessKey.req.length());
        lo = newLo;
        hi = newHi;
    }
    return -1;
}

ExDictionary::Name ExDictionary::getEntry(int dictIdx) const {
    if (!isLoaded()) {
        assert(false);
        return Name{};
    }
    return cmds[dictIdx];
}

void ExDictionary::rebuild() {
    cmds.clear();
    
    const char* begin = filebuf.get();
    while (*begin) {
        cmds.resize(cmds.size() + 1);
        Name& name = cmds.back();

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

static int less(const ExDictionary::Name& lhs, const ExDictionary::Name& rhs) {
    return lhs.req < rhs.req;
}

bool ExDictionary::debugCheckSorted() {
    bool sorted = true;
    for (int i = 1; i < cmds.size(); ++i) {
        if (!less(cmds[i - 1], cmds[i])) {
            sorted = false;
            break;
        }
    }
    if (sorted) {
        return true;
    }

    fputs("Dictionary is not sorted. This is the expected order:\n", stdout);
    std::sort(cmds.begin(), cmds.end(), less);
    for (int i = 0; i < cmds.size(); ++i) {
        printf("%.*s", cmds[i].req.length(), cmds[i].req.begin);
        printf("[%.*s]", cmds[i].opt.length(), cmds[i].opt.begin);
        fputc('\n', stdout);
    }
    return false;
}
