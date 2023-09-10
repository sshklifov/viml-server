#include <ExLexer.hpp>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <cassert>
#include <cctype>

#include "Command.hpp"
#include "ExDictionary.hpp"

bool ExLexer::buildExLexem(StringView line, LocationMap::Key locationKey, ExLexem& lex) {
    lex.exDictIdx = -1;
    lex.name.begin = lex.name.end;
    lex.qargs.begin = lex.qargs.end;
    lex.locationKey = locationKey;
    lex.nameOffset = 0;
    lex.qargsOffset = 0;
    lex.bang = 0;
    lex.range = 0;

    YY_BUFFER_STATE buf = cmd_scan_bytes(line.begin, line.length());
    const ExDictionary& dict = ExDictionary::getSingleton();

    enum {ERROR=256, RANGE_ARG, RANGE_DELIM, COMMAND_COLON, COMMAND, COMMAND_SPECIAL};
    int lineOffset = 0;
    int done = false;
    // TODO handle command colon order (must be before cmd)
    do {
        int tok = cmdlex();
        switch (tok) {
            case '\0':
                done = true;
                break;

            case '\t':
            case ' ':
                // Increment line offset and break
                break;

            case ERROR:
                reporter->error(format("Unexpected token {}", *cmdget_text()), locationKey, lineOffset, lineOffset);
                return false;

            case RANGE_ARG:
            case RANGE_DELIM:
                lex.range = 1;
                break;

            case COMMAND_SPECIAL:
            case COMMAND: {
                int cmdNameLen = 0;
                int dictIdx = dict.partialSearch(cmdget_text(), cmdNameLen);
                if (dictIdx < 0) {
                    reporter->error("Not an editor command", locationKey);
                    return false;
                } else {
                    StringView namePart(line.begin + lineOffset, cmdNameLen);
                    StringView restPart(namePart.end, namePart.begin + cmdget_leng());
                    if (!restPart.empty()) {
                        if (tok != COMMAND_SPECIAL && isalpha(restPart.left())) {
                            reporter->error("Not an editor command", locationKey);
                            return false;
                        } else if (restPart.left() == '!') {
                            ++restPart.begin;
                            lex.bang = true;
                        }
                    }
                    lex.exDictIdx = dictIdx;
                    lex.name = namePart;
                    lex.qargs = restPart.trimLeftSpace();
                    lex.nameOffset = lex.name.begin - line.begin;
                    lex.qargsOffset = lex.qargs.begin - line.begin;
                }
                break;
            }

        }
        lineOffset += cmdget_leng();
    }
    while (!done);

    cmd_delete_buffer(buf);
    return true;
}

bool ExLexer::reload(const char* str) {
    int n = strlen(str);
    program.set(str, n);
    contStorage.realloc(n);
    locationMap.clearFlagments();
    return true;
}

bool ExLexer::lex(ExLexem& res) {
    while (!program.empty()) {
        Continuation cont(contStorage, locationMap);
        cont.add(program.top(), program.lineNumber(), 0);
        program.pop();

        while (!program.empty()) {
            const char* lineBegin = program.top().begin;
            StringView contLine = program.top().trimLeftSpace();
            int codeCont = contLine.beginsWith('\\');
            int commentCont = contLine.beginsWith("\"\\ ");
            if (!codeCont && !commentCont) {
                break;
            }
            if (codeCont) {
                contLine = contLine.popLeft(); // Remove continuation character
                int colOffset = contLine.begin - lineBegin;
                cont.add(contLine, program.lineNumber(), colOffset);
            }
            program.pop();
        }

        LocationMap::Key locationKey;
        StringView joinedLine = cont.finish(locationKey);

        // Check if line is a comment or empty
        StringView tmpLine = joinedLine.trimLeftSpace();
        // Must include a line feed character (at least)
        assert(tmpLine.length() >= 1);
        bool ignore = (tmpLine.left() == '"' || tmpLine.left() == '\n');
        if (!ignore) {
            if (buildExLexem(joinedLine, locationKey, res)) {
                return true;
            }
        }
    }
    return false;
}

void ExLexer::setDiagnosticReporter(DiagnosticReporter& rep) {
    reporter = &rep;
}

const LocationMap& ExLexer::getLocationMap() const { return locationMap; }

// TODO carriage returns
