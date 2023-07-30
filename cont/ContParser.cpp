#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <cstddef>
#include <cstdio>
#include <cassert>

#include <string>

#include "ContParser.hpp"

extern struct yy_buffer_state* cont_scan_bytes(const char* yybytes, int yybytes_len);
extern void cont_delete_buffer (yy_buffer_state* b);

extern int contlex (void);

ContParser::Impl::Impl() {
    yybuffer = nullptr;
    mptr = nullptr;
    len = 0;
    fd = -1;
}

ContParser::Impl::~Impl() {
    unload();
}

bool ContParser::Impl::tryLoad(const char* filename) {
    int fd = open(filename, O_RDWR);
    if (fd < 0) {
        return false;
    }

    struct stat statbuf;
    int s = fstat(fd, &statbuf);
    if (s != 0) {
        close(fd);
        return false;
    }

    size_t len =  statbuf.st_size;
    void* mptr = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (!mptr) {
        close(fd);
        return false;
    }

    const char* buffer = reinterpret_cast<const char*>(mptr);
    yy_buffer_state* yybuffer = cont_scan_bytes(buffer, len);
    if (!yybuffer) {
        munmap(mptr, len);
        close(fd);
        return false;
    }

    // Success
    this->yybuffer = yybuffer;
    this->mptr = mptr;
    this->len = len;
    this->fd = fd;

    return true;
}

bool ContParser::Impl::unload() {
    if (isLoaded()) {
        cont_delete_buffer(reinterpret_cast<yy_buffer_state*>(yybuffer));
        yybuffer = nullptr;

        munmap(mptr, len);
        mptr = nullptr;

        close(fd);
        fd = -1;

        return true;
    }
    return false;
}

bool ContParser::Impl::isLoaded() const {
    return yybuffer != nullptr;
}

std::unique_ptr<char[]> ContParser::Impl::lex(int& resLen) {
    if (!isLoaded()) {
        return nullptr;
    }

    std::unique_ptr<char[]> res(new char[len]);
    for (int i = 0; i < len; ++i) {
        res[i] = contlex();
        if (res[i] == 0) {
            resLen = i;
            return res;
        }
    }
    resLen = len;

    unload();
    return res;
}
