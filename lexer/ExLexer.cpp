#include <ExLexer.hpp>

#include <EvalUtil.hpp>
#include <DoCmd.hpp>
#include <OptionDefs.hpp>
#include <Message.hpp>

#include <cassert>

bool ExLexer::reload(const char* str) {
    int n = strlen(str);
    program.set(str);
    contStorage.realloc(n);
    return true;
}

// TODO carriage returns

bool ExLexer::lexNext(ExLexem& res, const char*& nextcmd, DiagnosticReporter& rep) {
    if (nextcmd) {
        // "res" is a command that has a bar separator
        const char* cmdline = nextcmd;
        try {
            if (do_one_cmd(cmdline, nextcmd, res)) {
                return true;
            }
        } catch (msg& m) {
            int pos = m.ppos - cmdline;
            Range range = res.locator.resolve(pos, pos + 1);
            rep.error(std::move(m.message), range);
        }
    }
    // Parse a new ExLexem
    while (!program.empty()) {
        Locator locator;
        CmdlineCreator cmdlineCreator(contStorage, locator);

        cmdlineCreator.concat(program.top(), program.lineNumber(), 0);
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
                    cmdlineCreator.concat(line, program.lineNumber(), col);
                }
                program.pop();
            }
        }

        const char* cmdline = cmdlineCreator.finish();
        try {
            if (do_one_cmd(cmdline, nextcmd, res)) {
                res.cmdline = cmdline;
                res.locator = locator;
                return true;
            }
        } catch (msg& m) {
            int pos = m.ppos - cmdline;
            Range range = locator.resolve(pos, pos + 1);
            rep.error(std::move(m.message), range);
        }
    }
    return false;
}
