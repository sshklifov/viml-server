#include <ExLexer.hpp>

#include <DoCmd.hpp>
#include <OptionDefs.hpp>
#include <Message.hpp>

#include <cassert>

bool ExLexer::reload(const char* str) {
    int n = strlen(str);
    program.set(str, n);
    contStorage.realloc(n);
    return true;
}

// TODO carriage returns

bool ExLexer::lexNext(DiagnosticReporter& rep, ExLexem& res) {
    if (res.nextcmd) {
        // "res" is a command that should be continued
        StringView cmdline = res.cmdline;
        try {
            if (do_one_cmd(res.nextcmd, res)) {
                res.cmdline = cmdline;
                return true;
            }
        } catch (msg& m) {
            int pos = m.ppos - cmdline.begin;
            Range range = res.locator.resolve(pos, pos);
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
                StringView origLine = program.top();
                // Determine if origLine is a continuation
                StringView line = origLine.trimLeftSpace();
                int codeCont = (line[0] == '\\');
                int commentCont = (line[0] == '"' && line[1] == '\\' && line[2] == ' ');
                if (!codeCont && !commentCont) {
                    break;
                }
                if (codeCont) {
                    line = line.popLeft(); //< Remove continuation character
                    int col = line.begin - origLine.begin;
                    cmdlineCreator.concat(origLine, program.lineNumber(), col);
                }
                program.pop();
            }
        }

        StringView cmdline = cmdlineCreator.finish();
        try {
            if (do_one_cmd(cmdline.begin, res)) {
                res.cmdline = cmdline;
                res.locator = locator;
                return true;
            }
        } catch (msg& m) {
            int pos = m.ppos - cmdline.begin;
            Range range = locator.resolve(pos, pos);
            rep.error(std::move(m.message), range);
        }
    }
    return false;
}
