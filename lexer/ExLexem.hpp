#pragma once

#include <Range.hpp>
#include <Vector.hpp>
#include <DiagnosticReporter.hpp>

#include <ExCmdsDefs.hpp>
#include <ExCmdsEnum.hpp>

struct CmdlineResolver {
    struct Fragment {
        Fragment() = default;
        Fragment(int line, int col, int fragLen) : line(line), col(col), fragLen(fragLen) {}

        int line;
        int col;
        int fragLen;
    };

    CmdlineResolver() : cmdline(nullptr) {}

    CmdlineResolver(const char* cmd, Vector<Fragment> frags) : fragments(frags), cmdline(cmd) {}

    void addFragment(int line, int col, int entryLen) {
        fragments.emplace(line, col, entryLen);
    }

    void set(const char* cmd, Vector<Fragment> frags) {
        cmdline = cmd;
        fragments = std::move(frags);
    }

    Range resolve(const char* pos) const {
        return resolve(pos, pos + 1);
    }

    Range resolve(const char* begin, const char* end) const {
        return resolve(begin - cmdline, end - cmdline);
    }

    Range resolve(int begin, int end) const {
        assert(!fragments.empty());
        assert(begin < end);
        Range res;
        int strBegin = 0;
        for (int i = 0; i < fragments.count(); ++i) {
            int strEnd = strBegin + fragments[i].fragLen;
            if (begin >= strBegin && begin < strEnd) {
                res.start.line = fragments[i].line;
                res.start.character = fragments[i].col + (begin - strBegin);
            }
            if (end > strBegin && end <= strEnd) {
                res.end.line = fragments[i].line;
                res.end.character = fragments[i].col + (end - strBegin);
                return res;
            }
            strBegin = strEnd;
        }
        if (res.start.line == MAXLNUM) {
            res.start.line = fragments.last().line;
            res.start.character = fragments.last().col + fragments.last().fragLen - 1;
        }
        res.end.line = fragments.last().line;
        res.end.character = fragments.last().col + fragments.last().fragLen;
        return res;
    }

    Range resolve() const {
        assert(!fragments.empty());
        Range res;
        res.start.line = fragments[0].line;
        res.start.character = fragments[0].col;
        res.end.line = fragments.last().line;
        res.end.character = fragments.last().col + fragments.last().fragLen - 1;
        return res;
    }

    const char* reverseResolve(const Position& pos) {
        int offset = -1;
        int fragLen = 0;
        for (int i = 0; i < fragments.count(); ++i) {
            if (fragments[i].line == pos.line && fragments[i].col <= pos.character) {
                offset = fragLen + (pos.character - fragments[i].col);
            }
            fragLen += fragments[i].fragLen;
        }
        if (offset < 0) {
            return nullptr;
        } else {
            return cmdline + offset;
        }
    }

private:
    Vector<Fragment> fragments;
    const char* cmdline;
};

struct ExLexem : public exarg {
    ExLexem() {
        name = nullptr;
        qargs = nullptr;
        nextcmd = nullptr;

        namelen = 0;
        bang = false;
        range = false;
        cmdidx = CMD_SIZE;
    }

    CmdlineResolver locator;
};

struct BoundReporter {
    BoundReporter(DiagnosticReporter& rep, const ExLexem& lex) : rep(rep), lex(lex) {}

    void error(FStr msg, const char* pos) {
        Range range = lex.locator.resolve(pos);
        rep.error(std::move(msg), range);
    }

    void error(FStr msg) {
        Range range = lex.locator.resolve();
        rep.error(std::move(msg), range);
    }

    void error(msg& m) {
        Range range = lex.locator.resolve(m.ppos);
        rep.error(std::move(m.message), range);
    }

    void errorName(FStr msg) {
        Range range = lex.locator.resolve(lex.name, lex.name + lex.namelen);
        rep.error(std::move(msg), range);
    }

private:
    DiagnosticReporter& rep;
    const ExLexem& lex;
};
