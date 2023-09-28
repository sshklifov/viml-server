#include <ExLexer.hpp>

#include <DoCmd.hpp>
#include <Message.hpp>

#include <cassert>

bool ExLexer::reload(const char* str) {
    int n = strlen(str);
    program.set(str, n);
    contStorage.realloc(n);
    return true;
}

// TODO 'cpo' C
// TODO carriage returns

bool ExLexer::lex(DiagnosticReporter& rep, ExLexem& res) {
    while (!program.empty()) {
        Locator locator;
        CmdlineCreator cmdlineCreator(contStorage, locator);
        cmdlineCreator.concat(program.top(), program.lineNumber(), 0);
        program.pop();

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

        StringView cmdline = cmdlineCreator.finish();
        try {
            if (do_one_cmd(cmdline, res)) {
                res.locator = locator;
                return true;
            }
        } catch (msg& m) {
            rep.error(std::move(m.message), res, m.ppos);
            // TODO try to issue lexem?
        }
    }
    return false;
}
