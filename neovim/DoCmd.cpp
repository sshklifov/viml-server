#include "DoCmd.hpp"

#include "ExCmdsDefs.hpp"
#include "ExCmdsEnum.hpp"
#include "OptionDefs.hpp"

#include "Ascii.hpp"
#include "Charset.hpp"
#include "Eval.hpp"
#include "EvalUtil.hpp"
#include "Message.hpp"
#include "Mbyte.hpp"

#include <cstring>
#include <cassert>

// Whether a command index indicates a user command.
#define IS_USER_CMDIDX(idx) ((int)(idx) < 0)

/// Check for class item. "pp" points to the '['.
// Returns a pointer to after the item.
// If no item is found, skip the '['.
const char* skip_class(const char* pp) {
    /// Check for a character class name "[:name:]".
    const char* class_names[] = {
        "alnum:]",
        "alpha:]",
        "blank:]",
        "cntrl:]",
        "digit:]",
        "graph:]",
        "lower:]",
        "print:]",
        "punct:]",
        "space:]",
        "upper:]",
        "xdigit:]",
        "tab:]",
        "return:]",
        "backspace:]",
        "escape:]",
        "ident:]",
        "keyword:]",
        "fname:]",
    };
    if (pp[1] == ':') {
        for (int i = 0; i < 19; i++) {
            int n = strlen(class_names[i]);
            if (strncmp(pp + 2, class_names[i], n) == 0) {
                return pp + n + 2;
            }
        }
    }

    // Check for an equivalence class name "[=a=]".
    if (pp[1] == '=' && pp[2] != NUL) {
        if (pp[3] == '=' && pp[4] == ']')
        return pp += 5;
    }

    // Check for a collating element "[.a.]".  "pp" points to the '['.
    if (pp[1] == '.' && pp[2] != NUL) {
        if (pp[3] == '.' && pp[4] == ']') {
            return pp + 5;
        }
    }

    return pp + 1;
}

/// Skip over a "[]" range.
/// "p" must point to the character after the '['.
const char* skip_anyof(const char* p) {
    // REGEXP_INRANGE contains all characters which are always special in a []
    // range after '\'.
    // REGEXP_ABBR contains all characters which act as abbreviations after '\'.
    // These are:
    //  \n  - New line (NL).
    //  \r  - Carriage Return (CR).
    //  \t  - Tab (TAB).
    //  \e  - Escape (ESC).
    //  \b  - Backspace (Ctrl_H).
    //  \d  - Character code in decimal, eg \d123
    //  \o  - Character code in octal, eg \o80
    //  \x  - Character code in hex, eg \x4a
    //  \u  - Multibyte character code, eg \u20ac
    //  \U  - Long multibyte character code, eg \U12345678
    char REGEXP_INRANGE[] = "]^-n\\";
    char REGEXP_ABBR[] = "nrtebdoxuU";

    if (*p == '^') {  // Complement of range.
        p++;
    }
    if (*p == ']' || *p == '-') {
        p++;
    }
    while (*p != NUL && *p != ']') {
        if (*p == '-') {
            p++;
            if (*p != ']' && *p != NUL) {
                p++;
            }
        } else if (*p == '\\'
            && (strchr(REGEXP_INRANGE, p[1]) != NULL
                || (!reg_cpo_lit && strchr(REGEXP_ABBR, p[1]) != NULL))) {
            p += 2;
        } else if (*p == '[') {
            p = skip_class(p);
        } else {
            p++;
        }
    }

    if (*p == NUL) {
        throw msg("Expecting ]", p);
    }
    return p;
}

/// Skip past regular expression.
/// Stop at end of "startp" or where "delim" is found ('/', '?', etc).
/// Take care of characters with a backslash in front of it.
/// Skip strings inside [ and ].
const char* skip_regexp(const char* p, int delim) {
    magic_T mymagic = MAGIC_ON;
    while (*p != NUL && *p != delim) {
        if ((p[0] == '[' && mymagic >= MAGIC_ON) || (p[0] == '\\' && p[1] == '[' && mymagic <= MAGIC_OFF)) {
            p = skip_anyof(p + 1);
        } else if (p[0] == '\\' && p[1] != NUL) {
            p++; // skip next character
            if (*p == 'v') {
                mymagic = MAGIC_ALL;
            } else if (*p == 'V') {
                mymagic = MAGIC_NONE;
            }
        }
        p++;
    }
    if (*p == NUL) {
        throw msg(p, "Expecting {}", delim);
    }
    return p;
}

/// Skip over the pattern argument of ":vimgrep /pat/[g][j]".
const char* skip_vimgrep_pat(const char* p) {
    if (vim_isIDc(*p)) {
        // ":vimgrep pattern fname"
        p = skiptowhite(p);
    } else {
        // ":vimgrep /pattern/[g][j] fname"
        int delim = *p;
        p = skip_regexp(p + 1, delim);
        p++;
        // Find the flags
        while (*p == 'g' || *p == 'j' || *p == 'f') {
            p++;
        }
    }
    return p;
}

/// Skip colons and trailing whitespace, returning a pointer to the first
/// non-colon, non-whitespace character.
//
/// @param skipleadingwhite Skip leading whitespace too
const char* skip_colon_white(const char *p) {
    p = skipwhite(p);
    while (*p == ':') {
        p = skipwhite(p + 1);
    }

    return p;
}

/// Skip a range specifier of the form: addr [,addr] [;addr] ..
///
/// Backslashed delimiters after / or ? will be skipped, and commands will
/// not be expanded between /'s and ?'s or after "'".
///
/// Also skip white space and ":" characters.
///
/// @param addr_skipped true if any addrs was skipped
/// @return the "cmd" pointer advanced to beyond the range.
const char* skip_range(const char* cmd, int& has_range) {
    has_range = 0;
    while (strchr(" \t0123456789.$%'/?-+,;\\", (*cmd)) != NULL) {
        if (*cmd != ' ' && *cmd != '\t') {
            has_range = 1;
        }
        if (*cmd == '\\') {
            if (cmd[1] == '?' || cmd[1] == '/' || cmd[1] == '&') {
                cmd++;
            } else {
                throw msg(cmd, "\\ Should be followed by /, ? or &");
            }
        } else if (*cmd == '\'') {
            ++cmd;
        } else if (*cmd == '/' || *cmd == '?') {
            unsigned delim = (unsigned)(*cmd++);
            while (*cmd != NUL && *cmd != (char)delim) {
                if (*cmd++ == '\\' && *cmd != NUL) {
                    cmd++;
                }
            }
        }
        if (*cmd != NUL) {
            cmd++;
        }
    }

    return cmd;
}

/// Check for an Ex command with optional tail.
/// If there is a match advance "pp" to the argument and return true.
///
/// @param pp   start of command
/// @param cmd  name of command
/// @param len  required length
bool checkforcmd(const char*& pp, const char* cmd, int len) {
    int i;
    for (i = 0; cmd[i] != NUL; i++) {
        if (cmd[i] != pp[i]) {
            break;
        }
    }
    if (i >= len && !ASCII_ISALPHA(pp[i])) {
        pp = skipwhite(pp + i);
        return true;
    }
    return false;
}

int ends_excmd(int c) {
    return c == NUL || c == '|' || c == '"' || c == '\n';
}

const char* skip_command_modifiers(const char* cmd) {
    // Repeat until no more command modifiers are found.
    for (;;) {
        int has_mod = 0;
        int has_range = 0;
        int allow_range = 0;

        const char* p = skip_range(cmd, has_range);
        p = skip_colon_white(cmd);
        switch (*p) {
            // When adding an entry, also modify cmd_exists().
        case 'a':
            has_mod = checkforcmd(p, "aboveleft", 3);
            break;

        case 'b':
            has_mod = checkforcmd(p, "belowright", 3)
                || checkforcmd(p, "browse", 3)
                || checkforcmd(p, "botright", 2);
            break;

        case 'c':
            has_mod = checkforcmd(p, "confirm", 4);
            break;

        case 'k':
            has_mod = checkforcmd(p, "keepmarks", 3)
                || checkforcmd(p, "keepalt", 5)
                || checkforcmd(p, "keeppatterns", 5)
                || checkforcmd(p, "keepjumps", 5);
            break;

        case 'f': {  // only accept ":filter {pat} cmd"
            if (checkforcmd(p, "filter", 4)) {
                has_mod = true;
                if (*p == '!') {
                    p = skipwhite(p + 1);
                }
                if (ends_excmd(*p)) {
                    throw msg(p, "Argument required");
                }
                p = skip_vimgrep_pat(p);
            }
            break;
        }

        case 'h':
            has_mod = checkforcmd(p, "horizontal", 3);
            if (!has_mod) {
                // ":hide" and ":hide | cmd" are not modifiers
                const char* hide_mod = p;
                if (checkforcmd(hide_mod, "hide", 3) && !ends_excmd(*hide_mod)) {
                    has_mod = true;
                    p = hide_mod;
                }
            }
            break;

        case 'l':
            has_mod = checkforcmd(p, "lockmarks", 3)
                || checkforcmd(p, "leftabove", 5);
            break;

        case 'n':
            has_mod = checkforcmd(p, "noautocmd", 3)
                || checkforcmd(p, "noswapfile", 3);
            break;

        case 'r':
            has_mod = checkforcmd(p, "rightbelow", 6);
            break;

        case 's':
            has_mod = checkforcmd(p, "sandbox", 3);
            if (!has_mod) {
                has_mod = checkforcmd(p, "silent", 3);
                if (has_mod && *p == '!') {
                    // ":silent!", but not "silent !cmd"
                    p = skipwhite(p + 1);
                }
            }
            break;

        case 't':
            has_mod = checkforcmd(p, "topleft", 2);
            if (!has_mod) {
                has_mod = checkforcmd(p, "tab", 3);
                if (has_mod) {
                    allow_range = true;
                }
            }
            break;

        case 'u':
            has_mod = checkforcmd(p, "unsilent", 3);
            break;

        case 'v':
            has_mod = checkforcmd(p, "vertical", 4);
            if (!has_mod) {
                has_mod = checkforcmd(p, "verbose", 4);
                if (has_mod) {
                    allow_range = true;
                }
            }
            break;
        }
        if (!has_mod) {
            return cmd;
        }
        if (has_range && !allow_range) {
            throw msg("No range allowed", cmd);
        }
        cmd = p;
    }
}

/// Return true and set "*idx" if "p" points to a one letter command.
/// - The 'k' command can directly be followed by any character.
/// - The 's' command can be followed directly by 'c', 'g', 'i', 'I' or 'r'
///          but :sre[wind] is another command, as are :scr[iptnames],
///          :scs[cope], :sim[alt], :sig[ns] and :sil[ent].
int one_letter_cmd(const char *p, int* idx) {
    if (*p == 'k') {
        *idx = CMD_k;
        return true;
    }
    if (p[0] == 's'
        && ((p[1] == 'c'
                && (p[2] == NUL
                    || (p[2] != 's' && p[2] != 'r'
                        && (p[3] == NUL
                            || (p[3] != 'i' && p[4] != 'p')))))
            || p[1] == 'g'
            || (p[1] == 'i' && p[2] != 'm' && p[2] != 'l' && p[2] != 'g')
            || p[1] == 'I'
            || (p[1] == 'r' && p[2] != 'e'))) {
        *idx = CMD_substitute;
        return true;
    }
    return false;
}

/// Find an Ex command by its name, either built-in or user.
/// Start of the name can be found at cmd
/// Advances cmd to the end of name
///
/// @return cmdidx or -1 if user defined command
int find_ex_command(const char*& cmd, int& len) {
    // Isolate the command and search for it in the command table.
    int cmdidx = CMD_SIZE;
    const char* p = cmd;
    if (one_letter_cmd(p, &cmdidx)) {
        p++;
    }
    else {
        while (ASCII_ISALPHA(*p)) {
            p++;
        }
        // for python 3.x support ":py3", ":python3", ":py3file", etc.
        if (cmd[0] == 'p' && cmd[1] == 'y') {
            while (ASCII_ISALNUM(*p)) {
                p++;
            }
        }

        // check for non-alpha command
        if (p == cmd && strchr("@!=><&~#", *p) != NULL) {
            p++;
        }
        len = p - cmd;
        // The "d" command can directly be followed by 'l' or 'p' flag.
        if (*cmd == 'd' && (p[-1] == 'l' || p[-1] == 'p')) {
            // Check for ":dl", ":dell", etc. to ":deletel": that's
            // :delete with the 'l' flag.  Same for 'p'.
            int i;
            for (i = 0; i < len; i++) {
                if (cmd[i] != ("delete")[i]) {
                    break;
                }
            }
            if (i == len - 1) {
                len--;
            }
        }

        if (ASCII_ISLOWER(cmd[0])) {
            const int c1 = cmd[0];
            const int c2 = len == 1 ? NUL : cmd[1];

            // Use a precomputed index for fast look-up in cmdnames[]
            // taking into account the first 2 letters of eap->cmd.
            cmdidx = cmdidxs1[CHAR_ORD_LOW(c1)];
            if (ASCII_ISLOWER(c2)) {
                cmdidx += cmdidxs2[CHAR_ORD_LOW(c1)][CHAR_ORD_LOW(c2)];
            }
        } else if (ASCII_ISUPPER(cmd[0])) {
            cmdidx = CMD_Next;
        } else {
            cmdidx = CMD_bang;
        }
        assert(cmdidx >= 0);

        for (; cmdidx < CMD_SIZE; cmdidx++) {
            if (strncmp(cmdnames[cmdidx].cmd_name, cmd, len) == 0) {
                break;
            }
        }

        // Look for a user defined command as a last resort.
        if (cmdidx == CMD_SIZE) {
            if (*cmd >= 'A' && *cmd <= 'Z') {
                return -1; // TODO
            } else {
                throw msg(cmd, "Not an editor command");
            }
        }
    }

    cmd = p;
    return cmdidx;
}

/// Find end of "+command" argument.  Skip over "\ " and "\\".
const char *skip_cmd_arg(const char* p) {
    while (*p && !ascii_isspace(*p)) {
        if (*p == '\\' && p[1] != NUL) {
            p++;
        }
        p++;
    }
    return p;
}

/// Get "++opt=arg" argument.
///
/// @return  pointer to past the argument
const char* getargopt(const char* arg, int cmdidx) {
    const char* p = arg + 2;
    char opt = NUL;

    // ":edit ++[no]bin[ary] file"
    if (strncmp(p, "bin", 3) == 0 || strncmp(p, "nobin", 5) == 0) {
        if (*p == 'n') {
            p += 2;
        }
        if (!checkforcmd(p, "binary", 3)) {
            throw msg(arg, "Invalid ++opt");
        }
        return skipwhite(p);
    }

    // ":read ++edit file"
    if (strncmp(p, "edit", 4) == 0) {
        if (cmdidx != CMD_read) {
            throw msg(arg, "++edit is for :read only");
        }
        return skipwhite(p + 4);
    }

    // ":write ++p foo/bar/file
    if (strncmp(p, "p", 1) == 0) {
        if (cmdidx != CMD_write) {
            throw msg(arg, "++p is for :write only");
        }
        return skipwhite(p + 1);
    }

    if (strncmp(p, "ff", 2) == 0) {
        opt = *p;
        p += 2;
    } else if (strncmp(p, "fileformat", 10) == 0) {
        opt = *p;
        p += 10;
    } else if (strncmp(p, "enc", 3) == 0) {
        opt = *p;
        if (strncmp(p, "encoding", 8) == 0) {
            p += 8;
        } else {
            p += 3;
        }
    } else if (strncmp(p, "bad", 3) == 0) {
        opt = *p;
        p += 3;
    } else {
        throw msg(arg, "Invalid ++opt");
    }
    assert(opt);

    if (*p != '=') {
        throw msg(p, "Expecting '='");
    }
    p++;
    if (ascii_iswhite(*p)) {
        throw msg(p, "Trailing whitespace");
    }

    int len = skip_cmd_arg(p) - p;
    if (opt == 'f') {
        if (strcmp(p, "unix") != 0 || len != 4) {
            if (strcmp(p, "dos") != 0 || len != 3) {
                if (strcmp(p, "mac") != 0 || len != 3) {
                    throw msg(p, "Invalid fileformat");
                }
            }
        }
    } else if (opt == 'e') {
        if (!valid_encoding_len(p, len)) {
            throw msg(p, "Invalid encoding");
        }
    } else {
        // Check ++bad= argument.  Must be a single-byte character, "keep" or "drop".
        if (strcasecmp(p, "keep") != 0 || len != 4) {
            if (strcasecmp(p, "drop") != 0 || len != 4) {
                if (len != 1) {
                    throw msg(p, "Invalid valie for ++bad");
                }
            }
        }
    }
    return skipwhite(p + len);
}

/// get + command from ex argument
const char* skip_argcmd(const char* arg) {
    // TODO checking
    if (*arg == '+') {        // +[command]
        arg = skip_cmd_arg(arg + 1);
        arg = skipwhite(arg);
    }
    return arg;
}

const char* skip_register(const char* cmdline, int cmdidx) {
    int cmd_argt = cmdnames[cmdidx].cmd_argt;
    // Do not allow register = for user commands
    if (IS_USER_CMDIDX(cmdidx) && *cmdline == '=') {
        return cmdline;
    }
    // Accept numbered register only when no count allowed (:put)
    if ((cmd_argt & EX_COUNT) && ascii_isdigit(*cmdline)) {
        return cmdline;
    }

    if ((cmd_argt & EX_REGSTR) && *cmdline != NUL) {
        int writing = (cmdidx != CMD_put && !IS_USER_CMDIDX(cmdidx));
        if (valid_yank_reg(*cmdline, writing)) {
            // for '=' register: the rest of the line is an expression
            if (cmdline[0] != '=' || cmdline[1] == NUL) {
                ++cmdline;
                cmdline = skipwhite(cmdline);
            }
        }
    }
    return cmdline;
}

const char* skip_count(const char* cmdline, int cmdidx) {
    int cmd_argt = cmdnames[cmdidx].cmd_argt;
    // Check for a count
    if (!(cmd_argt & EX_COUNT) || !ascii_isdigit(*cmdline)) {
        return cmdline;
    }

    const char* p = skipdigits(cmdline);
    // When accepting a EX_BUFNAME, don't use "123foo" as a count,
    // it's a buffer name.
    if (cmd_argt & EX_BUFNAME) {
        if (*p != NUL && !ascii_iswhite(*p)) {
            return cmdline;
        }
    }
    return skipwhite(p);
}

/// Get flags from an Ex command argument.
const char* skip_flags(const char* cmdline, int cmdidx) {
    int cmd_argt = cmdnames[cmdidx].cmd_argt;
    // Check for flags
    if (!(cmd_argt & EX_FLAGS)) {
        return cmdline;
    }

    while (strchr("lp#", *cmdline) != NULL) {
        cmdline++;
        if (ascii_iswhite(*cmdline)) {
            throw msg(cmdline, "Trailing whitespace");

        }
    }
    return skipwhite(cmdline);
}

/// Check for '|' to separate commands and '"' to start comments.
/// "cmdline" is advanced to the end of the command
const char* separate_nextcmd(const char*& cmdline, int cmdidx) {
    int isgrep = cmdidx == CMD_vimgrep 
        || cmdidx == CMD_lvimgrep
        || cmdidx == CMD_vimgrepadd
        || cmdidx == CMD_lvimgrepadd
        || cmdidx == CMD_grep
        || cmdidx == CMD_lgrep
        || cmdidx == CMD_grepadd
        || cmdidx == CMD_lgrepadd;

    const char* p = cmdline;
    if (*p != NUL && isgrep) {
        p = skip_vimgrep_pat(p);
    }

    int cmd_argt = cmdnames[cmdidx].cmd_argt;
    for (; *p; p++) {
        if (*p == Ctrl_V) {
            p++;  // skip CTRL-V and next char
            if (*p == NUL) {  // stop at NUL after CTRL-V
                break;
            }
        } else if (p[0] == '`' && p[1] == '=' && (cmd_argt & EX_XFILE)) {
            // Skip over `=expr` when wildcards are expanded.
            p = skip_expr(p + 2);
            if (*p != '`') {
                throw msg(p, "Expecting '`'");
            }
        } else {
            // Check for '"': start of comment or '|': next command */
            // :@" does not start a comment!
            // :redir @" doesn't either.
            bool comment_start = (*p  == '"')
                && !(cmd_argt & EX_NOTRLCOM)
                && (cmdidx != CMD_at || p != cmdline)
                && (cmdidx != CMD_redir || p != cmdline + 1 || p[-1] != '@');

            if (comment_start || *p == '|' || *p == '\n') {
                // We remove the '\' before the '|', unless EX_CTRLV is used
                // AND 'b' is present in 'cpoptions'.
                if (p[-1] != '\\' || (cpo_bar && (cmd_argt & EX_CTRLV))) {
                    cmdline = p;
                    if (*p == '|') {
                        return cmdline + 1;
                    } else {
                        return NULL;
                    }
                }
            }
        }
    }
    assert(*p == NUL);
    cmdline = p;
    return NULL;
}

int do_one_cmd(StringView cmdView, ExLexem& lexem) {
    const char* cmdline = cmdView.begin;

    // "#!anything" is handled like a comment.
    if (cmdline[0] == '#' && cmdline[1] == '!') {
        return false;
    }

    // 1. Skip comment lines and leading white space and colons.
    // 2. Handle command modifiers.
    cmdline = skip_command_modifiers(cmdline);

    // 3. Skip over the range to find the command.
    int has_range = 0;
    const char* range_pos = NULL;
    if (*skip_colon_white(cmdline) == '*') { // Reuse visual area as range
        has_range = 1;
        range_pos = skip_colon_white(cmdline);
        cmdline = skipwhite(range_pos + 1);
    } else {
        range_pos = skipwhite(cmdline);
        cmdline = skip_range(range_pos, has_range);
        cmdline = skip_colon_white(cmdline);
    }

    // ignore comment and empty lines
    if (ends_excmd(*skipwhite(cmdline))) {
        return false; // TODO!
    }

    const char* name_pos = cmdline;
    int name_len = 0;
    int cmdidx = find_ex_command(cmdline, name_len);
    if (cmdidx < 0) {
        // ignore user commands
        return false;
    }

    // 6. Parse arguments.  Then check for errors.
    int cmd_argt = cmdnames[cmdidx].cmd_argt;
    if (!(cmd_argt & EX_RANGE) && has_range) {
        throw msg(range_pos, "No range allowed");
    }

    // Forced commands.
    const char* bang_pos = NULL;
    int forceit = *cmdline == '!'
        && cmdidx != CMD_substitute
        && cmdidx != CMD_smagic
        && cmdidx != CMD_snomagic;
    if (forceit) {
        bang_pos = cmdline;
        cmdline++;
    }

    // Skip to start of argument.
    // Don't do this for the ":!" command, because ":!! -l" needs the space.
    cmdline = (cmdidx == CMD_bang ? cmdline : skipwhite(cmdline));

    // Check for "++opt=val" argument.
    // Must be first, allow ":w ++enc=utf8 !cmd"
    if (cmd_argt & EX_ARGOPT) {
        while (cmdline[0] == '+' && cmdline[1] == '+') {
            cmdline = getargopt(cmdline, cmdidx);
        }
    }

    int usefilter = false;
    if (cmdidx == CMD_write || cmdidx == CMD_update) {
        if (*cmdline == '>') {                       // append
            if (*++cmdline != '>') {                   // typed wrong
                throw msg(cmdline, "Use w or w>>");
            }
            cmdline = skipwhite(cmdline + 1);
        } else if (*cmdline == '!' && cmdidx == CMD_write) {  // :w !filter
            cmdline++;
            usefilter = true;
        }
    } else if (cmdidx == CMD_read) {
        if (forceit) {
            usefilter = true;                      // :r! filter if forceit
            forceit = false;
        } else if (*cmdline == '!') {            // :r !filter
            cmdline++;
            usefilter = true;
        }
    } else if (cmdidx == CMD_lshift || cmdidx == CMD_rshift) {
        char shift = cmdnames[cmdidx].cmd_name[0];
        while (*cmdline == shift) {                // count number of '>' or '<'
            cmdline++;
        }
        cmdline = skipwhite(cmdline);
    }
    if (!(cmd_argt & EX_BANG) && forceit) {
        throw msg(bang_pos, "No ! allowed");
    }

    // Check for "+command" argument, before checking for next command.
    // Don't do this for ":read !cmd" and ":write !cmd".
    if ((cmd_argt & EX_CMDARG) && !usefilter) {
        cmdline = skip_argcmd(cmdline);
    }

    // Parse register and count
    cmdline = skip_register(cmdline, cmdidx);
    cmdline = skip_count(cmdline, cmdidx);
    cmdline = skip_flags(cmdline, cmdidx);

    if (!(cmd_argt & EX_EXTRA) && !ends_excmd(*cmdline)) {
        throw msg(cmdline, "Trailing characters");
    }
    if ((cmd_argt & EX_NEEDARG) && ends_excmd(*cmdline)) {
        throw msg(cmdline, "Argument required");
    }

    // Check for '|' to separate commands and '"' to start comments.
    // Don't do this for ":read !cmd" and ":write !cmd".
    const char* cmdend = NULL;
    const char* nextcmd = NULL;
    if ((cmd_argt & EX_TRLBAR) && !usefilter) {
        cmdend = cmdline;
        nextcmd = separate_nextcmd(cmdend, cmdidx);
    } else {
        cmdend = cmdView.end;
    }

    // 6.9. Fill in ExLexem attributes
    lexem.bang = forceit;
    lexem.range = has_range;
    lexem.cmdidx = cmdidx;
    lexem.nextcmd = nextcmd;
    lexem.cmdline = cmdView;
    lexem.name = StringView(name_pos, name_len);
    lexem.qargs = StringView(cmdline, cmdend);
    return true;

#if 0
  // TODO HASSLE

  // If filename expansion is enabled, expand filenames
  if (cmd_argt & EX_XFILE) {
    if (expand_filename(eap, eap->cmdlinep, errormsg) == FAIL) {
      return FAIL;
    }
  }
#endif

#if 0
  // TODO HASSLE

  // Accept buffer name.  Cannot be used at the same time with a buffer
  // number.  Don't do this for a user command.
  if ((eap->argt & EX_BUFNAME) && *eap->arg != NUL && eap->addr_count == 0
      && !IS_USER_CMDIDX(eap->cmdidx)) {
    if (eap->args == NULL) {
      // If argument positions are not specified, search the argument for the buffer name.
      // :bdelete, :bwipeout and :bunload take several arguments, separated by spaces:
      // find next space (skipping over escaped characters).
      // The others take one argument: ignore trailing spaces.
      char *p;

      if (eap->cmdidx == CMD_bdelete || eap->cmdidx == CMD_bwipeout
          || eap->cmdidx == CMD_bunload) {
        p = skiptowhite_esc(eap->arg);
      } else {
        p = eap->arg + strlen(eap->arg);
        while (p > eap->arg && ascii_iswhite(p[-1])) {
          p--;
        }
      }
      eap->line2 = buflist_findpat(eap->arg, p, (eap->argt & EX_BUFUNL) != 0,
                                   false, false);
      eap->addr_count = 1;
      eap->arg = skipwhite(p);
    } else {
      // If argument positions are specified, just use the first argument
      eap->line2 = buflist_findpat(eap->args[0],
                                   eap->args[0] + eap->arglens[0],
                                   (eap->argt & EX_BUFUNL) != 0, false, false);
      eap->addr_count = 1;
      shift_cmd_args(eap);
    }
    if (eap->line2 < 0) {  // failed
      return FAIL;
    }
  }
#endif
}
