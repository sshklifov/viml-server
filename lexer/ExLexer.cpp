#include <ExLexer.hpp>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <cassert>
#include <cctype>

#include "Command.hpp"
#include "ExDictionary.hpp"

#if 0
files: ex_cmds.lua
src/nvim/eval.c

notables:
/// skip_regexp() with extra arguments:
/// When "newp" is not NULL and "dirc" is '?', make an allocated copy of the
/// expression and change "\?" to "?".  If "*newp" is not NULL the expression
/// is changed in-place.
/// If a "\?" is changed to "?" then "dropped" is incremented, unless NULL.
/// If "magic_val" is not NULL, returns the effective magicness of the pattern
char *skip_regexp_ex(char *startp, int dirc, int magic, char **newp, int *dropped,
                     magic_T *magic_val)
{
  magic_T mymagic;
  char *p = startp;

  if (magic) {
    mymagic = MAGIC_ON;
  } else {
    mymagic = MAGIC_OFF;
  }
  get_cpo_flags();

  for (; p[0] != NUL; MB_PTR_ADV(p)) {
    if (p[0] == dirc) {         // found end of regexp
      break;
    }
    if ((p[0] == '[' && mymagic >= MAGIC_ON)
        || (p[0] == '\\' && p[1] == '[' && mymagic <= MAGIC_OFF)) {
      p = skip_anyof(p + 1);
      if (p[0] == NUL) {
        break;
      }
    } else if (p[0] == '\\' && p[1] != NUL) {
      if (dirc == '?' && newp != NULL && p[1] == '?') {
        // change "\?" to "?", make a copy first.
        if (*newp == NULL) {
          *newp = xstrdup(startp);
          p = *newp + (p - startp);
        }
        if (dropped != NULL) {
          (*dropped)++;
        }
        STRMOVE(p, p + 1);
      } else {
        p++;            // skip next character
      }
      if (*p == 'v') {
        mymagic = MAGIC_ALL;
      } else if (*p == 'V') {
        mymagic = MAGIC_NONE;
      }
    }
  }
  if (magic_val != NULL) {
    *magic_val = mymagic;
  }
  return p;
}

if (*p != delim) {
  semsg(_("E654: missing delimiter after search pattern: %s"), startp);
  return NULL;
}

char *skip_vimgrep_pat(char *p, char **s, int *flags)

  if (*p != NUL && (eap->cmdidx == CMD_vimgrep || eap->cmdidx == CMD_lvimgrep
                    || eap->cmdidx == CMD_vimgrepadd
                    || eap->cmdidx == CMD_lvimgrepadd
                    || grep_internal(eap->cmdidx))) {

// Return true when using ":vimgrep" for ":grep".
int grep_internal(cmdidx_T cmdidx)
{
  return (cmdidx == CMD_grep
          || cmdidx == CMD_lgrep
          || cmdidx == CMD_grepadd
          || cmdidx == CMD_lgrepadd)
         && strcmp("internal", *curbuf->b_p_gp == NUL ? p_gp : curbuf->b_p_gp) == 0;
}

/// Check for '|' to separate commands and '"' to start comments.
void separate_nextcmd(exarg_T *eap)

#endif

int checkTrailingBar(int exDictIdx, StringView args) {
    // TODO ExDictionary extend
    return -1;

    /* enum State {NONE, INSIDE_STR, INSIDE_QUOTE} state = NONE; */
    /* for (int i = 0; i < line.length(); ++i) { */
    /*     char c = line[i]; */
    /*     switch (state) { */
    /*     case NONE: */
    /*         if (c == '\'') { */
    /*             state = INSIDE_QUOTE; */
    /*         } else if (c == '\"') { */
    /*             state = INSIDE_STR; */
    /*         } else if (c == '/') { */
    /*             state = INSIDE_PAT */
    /*         } */
    /*     } */
    /* } */
}

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
