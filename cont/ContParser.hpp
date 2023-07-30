#pragma once

#include <cstddef>

struct ContParser {
private:
    struct Impl {
        Impl();
        ~Impl();

        bool tryLoad(const char* filename);
        bool isLoaded() const;

        int lex();

    private:
        void* yybuffer;
        void* mptr;
        size_t len;
        int fd;
    };

public:
    static Impl& Get() {
        static Impl instance;
        return instance;
    }
};
