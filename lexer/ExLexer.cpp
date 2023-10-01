#include <ExLexer.hpp>

#include <Eval.hpp>
#include <DoCmd.hpp>
#include <Config.hpp>
#include <Message.hpp>

#include <cstring>
#include <cassert>

bool ExLexer::reload(const char* str) {
    int n = strlen(str);
    program.set(str);
    contStorage.realloc(n);
    return true;
}

// TODO carriage returns

bool ExLexer::lexNext(ExLexem& res, DiagnosticReporter& rep) {
    if (res.nextcmd) {
        // "res" is a command that has a bar separator
        const char* cmdline = res.nextcmd;
        try {
            if (do_one_cmd(cmdline, res)) {
                return true;
            }
        } catch (msg& m) {
            int pos = m.ppos - cmdline;
            Range range = res.locator.resolve(pos, pos + 1);
            rep.error(std::move(m.message), range);
        }
    }
    // Parse a new ExLexem
    for (;;) {
        // Skip empty lines. NB top() will return '\0' if empty
        while (*program.top() == '\n') {
            program.pop();
        }
        if (program.empty()) {
            return false;
        }

        CmdlineCreator creator(contStorage);
        creator.concat(program.top(), program.lineNumber(), 0);
        program.pop();

        if (!cpo_no_cont) {
            while (!program.empty()) {
                // Check if continued
                const char* line = skipwhite(program.top());
                int codeCont = (line[0] == '\\');
                int commentCont = (line[0] == '"' && line[1] == '\\' && line[2] == ' ');
                if (!codeCont && !commentCont) {
                    break;
                }
                if (codeCont) {
                    ++line; //< Remove continuation character
                    int col = line - program.top();
                    creator.concat(line, program.lineNumber(), col);
                }
                program.pop();
            }
        }

        CmdlineResolver resolver;
        const char* cmdline = creator.finish(resolver);
        try {
            if (do_one_cmd(cmdline, res)) {
                res.locator = std::move(resolver);
                return true;
            }
        } catch (msg& m) {
            int pos = m.ppos - cmdline;
            Range range = resolver.resolve(pos, pos + 1);
            rep.error(std::move(m.message), range);
        }
    }
}
