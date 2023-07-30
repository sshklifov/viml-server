#pragma once

#include <memory>

struct ContParser {
private:
    struct Impl {
        Impl();
        ~Impl();

        bool tryLoad(const char* filename);
        bool isLoaded() const;
        bool unload();

        std::unique_ptr<char[]> lex(int& resLen);

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
