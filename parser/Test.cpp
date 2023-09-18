#include <cstring>
#include <type_traits>

/// Magicness of a pattern, used by regexp code.
/// The order and values matter:
///  magic <= MAGIC_OFF includes MAGIC_NONE
///  magic >= MAGIC_ON  includes MAGIC_ALL
typedef enum {
  MAGIC_NONE = 1,  ///< "\V" very unmagic
  MAGIC_OFF = 2,   ///< "\M" or 'magic' off
  MAGIC_ON = 3,    ///< "\m" or 'magic'
  MAGIC_ALL = 4,   ///< "\v" very magic
} magic_T;

static int reg_cpo_lit = 0; // TODO

/// Check that "c" is a normal identifier character:
/// Letters and characters from the 'isident' option.
///
/// @param  c  character to check
bool vim_isIDc(int c) {
    // TODO
  return c > 0 && c < 0x100;
}

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
static char REGEXP_INRANGE[] = "]^-n\\";
static char REGEXP_ABBR[] = "nrtebdoxuU";

/// Check for a character class name "[:name:]".  "pp" points to the '['.
/// Returns one of the CLASS_ items. CLASS_NONE means that no item was
/// recognized.  Otherwise "pp" is advanced to after the item.
static int get_char_class(char **pp)
{
  static const char *(class_names[]) = {
    "alnum:]",
#define CLASS_ALNUM 0
    "alpha:]",
#define CLASS_ALPHA 1
    "blank:]",
#define CLASS_BLANK 2
    "cntrl:]",
#define CLASS_CNTRL 3
    "digit:]",
#define CLASS_DIGIT 4
    "graph:]",
#define CLASS_GRAPH 5
    "lower:]",
#define CLASS_LOWER 6
    "print:]",
#define CLASS_PRINT 7
    "punct:]",
#define CLASS_PUNCT 8
    "space:]",
#define CLASS_SPACE 9
    "upper:]",
#define CLASS_UPPER 10
    "xdigit:]",
#define CLASS_XDIGIT 11
    "tab:]",
#define CLASS_TAB 12
    "return:]",
#define CLASS_RETURN 13
    "backspace:]",
#define CLASS_BACKSPACE 14
    "escape:]",
#define CLASS_ESCAPE 15
    "ident:]",
#define CLASS_IDENT 16
    "keyword:]",
#define CLASS_KEYWORD 17
    "fname:]",
#define CLASS_FNAME 18
  };
#define CLASS_NONE 99
  int i;

  if ((*pp)[1] == ':') {
    for (i = 0; i < std::rank<decltype(class_names)>(); i++) {
      if (strncmp(*pp + 2, class_names[i], strlen(class_names[i])) == 0) {
        *pp += strlen(class_names[i]) + 2;
        return i;
      }
    }
  }
  return CLASS_NONE;
}

// Check for an equivalence class name "[=a=]".  "pp" points to the '['.
// Returns a character representing the class. Zero means that no item was
// recognized.  Otherwise "pp" is advanced to after the item.
static int get_equi_class(char **pp)
{
  int c;
  int l = 1;
  char *p = *pp;

  if (p[1] == '=' && p[2] != '\0') {
    l = strlen(p + 2);
    if (p[l + 2] == '=' && p[l + 3] == ']') {
      c = strlen(p + 2);
      *pp += l + 4;
      return c;
    }
  }
  return 0;
}

// Check for a collating element "[.a.]".  "pp" points to the '['.
// Returns a character. Zero means that no item was recognized.  Otherwise
// "pp" is advanced to after the item.
// Currently only single characters are recognized!
static int get_coll_element(char **pp)
{
  int c;
  int l = 1;
  char *p = *pp;

  if (p[0] != '\0' && p[1] == '.' && p[2] != '\0') {
    l = strlen(p + 2);
    if (p[l + 2] == '.' && p[l + 3] == ']') {
      c = strlen(p + 2);
      *pp += l + 4;
      return c;
    }
  }
  return 0;
}

/// Skip over a "[]" range.
/// "p" must point to the character after the '['.
/// The returned pointer is on the matching ']', or the terminating NUL.
static char *skip_anyof(char *p)
{
  int l;

  if (*p == '^') {  // Complement of range.
    p++;
  }
  if (*p == ']' || *p == '-') {
    p++;
  }
  while (*p != '\0' && *p != ']') {
    if ((l = strlen(p)) > 1) {
      p += l;
    } else if (*p == '-') {
      p++;
      if (*p != ']' && *p != '\0') {
        p++;
      }
    } else if (*p == '\\'
               && (strchr(REGEXP_INRANGE, p[1]) != NULL
                   || (!reg_cpo_lit
                       && strchr(REGEXP_ABBR, p[1]) != NULL))) {
      p += 2;
    } else if (*p == '[') {
      if (get_char_class(&p) == CLASS_NONE
          && get_equi_class(&p) == 0
          && get_coll_element(&p) == 0
          && *p != '\0') {
        p++;          // It is not a class name and not NUL
      }
    } else {
      p++;
    }
  }

  return p;
}

/// skip_regexp() with extra arguments:
/// When "newp" is not NULL and "dirc" is '?', make an allocated copy of the
/// expression and change "\?" to "?".  If "*newp" is not NULL the expression
/// is changed in-place.
/// If a "\?" is changed to "?" then "dropped" is incremented, unless NULL.
/// If "magic_val" is not NULL, returns the effective magicness of the pattern
char *skip_regexp_ex(
    char *startp,
    int dirc,
    int magic,
    char **newp,
    int *dropped,
    magic_T *magic_val
) {
  magic_T mymagic;
  char *p = startp;

  if (magic) {
    mymagic = MAGIC_ON;
  } else {
    mymagic = MAGIC_OFF;
  }
  /* get_cpo_flags(); */

  for (; p[0] != '\0'; p++) {
    if (p[0] == dirc) { // found end of regexp
      break;
    }
    if ((p[0] == '[' && mymagic >= MAGIC_ON) ||
        (p[0] == '\\' && p[1] == '[' && mymagic <= MAGIC_OFF)) {
      p = skip_anyof(p + 1);
      if (p[0] == '\0') {
        break;
      }
    } else if (p[0] == '\\' && p[1] != '\0') {
      if (dirc == '?' && newp != NULL && p[1] == '?') {
        // change "\?" to "?", make a copy first.
        if (*newp == NULL) {
          *newp = strdup(startp);
          p = *newp + (p - startp);
        }
        if (dropped != NULL) {
          (*dropped)++;
        }
        memmove((p), (p + 1), strlen(p + 1) + 1);
      } else {
        p++; // skip next character
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

/// Skip past regular expression.
/// Stop at end of "startp" or where "delim" is found ('/', '?', etc).
/// Take care of characters with a backslash in front of it.
/// Skip strings inside [ and ].
char *skip_regexp(char *startp, int delim, int magic)
{
  return skip_regexp_ex(startp, delim, magic, NULL, NULL, NULL);
}

/// Call skip_regexp() and when the delimiter does not match give an error and
/// return NULL.
char *skip_regexp_err(char *startp, int delim, int magic)
{
  char *p = skip_regexp(startp, delim, magic);

  if (*p != delim) {
    // TODO
    /* semsg(_("E654: missing delimiter after search pattern: %s"), startp); */
    return NULL;
  }
  return p;
}

/// Skip over text until ' ' or '\t' or NUL
///
/// @param[in]  p  Text to skip over.
///
/// @return Pointer to the next whitespace or NUL character.
char *skiptowhite(const char *p)
{
  while (*p != ' ' && *p != '\t' && *p != '\0') {
    p++;
  }
  return (char *)p;
}

// flags for skip_vimgrep_pat()
#define VGR_GLOBAL      1
#define VGR_NOJUMP      2
#define VGR_FUZZY       4

/// Skip over the pattern argument of ":vimgrep /pat/[g][j]".
/// Put the start of the pattern in "*s", unless "s" is NULL.
///
/// @param flags  if not NULL, put the flags in it: VGR_GLOBAL, VGR_NOJUMP.
/// @param s      if not NULL, terminate the pattern with a NUL.
///
/// @return  a pointer to the char just past the pattern plus flags.
char *skip_vimgrep_pat(char *p, char **s, int *flags)
{
  if (vim_isIDc((*p))) {
    // ":vimgrep pattern fname"
    if (s != NULL) {
      *s = p;
    }
    p = skiptowhite(p);
    if (s != NULL && *p != '\0') {
      *p++ = '\0';
    }
  } else {
    // ":vimgrep /pattern/[g][j] fname"
    if (s != NULL) {
      *s = p + 1;
    }
    int c = (*p);
    p = skip_regexp(p + 1, c, true);
    if (*p != c) {
      return NULL;
    }

    // Truncate the pattern.
    if (s != NULL) {
      *p = '\0';
    }
    p++;

    // Find the flags
    while (*p == 'g' || *p == 'j' || *p == 'f') {
      if (flags != NULL) {
        if (*p == 'g') {
          *flags |= VGR_GLOBAL;
        } else if (*p == 'j') {
          *flags |= VGR_NOJUMP;
        } else {
          *flags |= VGR_FUZZY;
        }
      }
      p++;
    }
  }
  return p;
}

/// For a ":vimgrep" or ":vimgrepadd" command return a pointer past the
/// pattern.  Otherwise return eap->arg.
static char *skip_grep_pat(exarg *eap)
{
  char *p = eap->arg;

  if (*p != '\0' && (eap->cmdidx == CMD_vimgrep || eap->cmdidx == CMD_lvimgrep
                    || eap->cmdidx == CMD_vimgrepadd
                    || eap->cmdidx == CMD_lvimgrepadd
                    || grep_internal(eap->cmdidx))) {
    p = skip_vimgrep_pat(p, NULL, NULL);
    if (p == NULL) {
      p = eap->arg;
    }
  }
  return p;
}

// TODO
/// Arguments used for Ex commands.
struct exarg {
  char *arg;                    ///< argument of the command
  char **args;                  ///< starting position of command arguments
  size_t *arglens;              ///< length of command arguments
  size_t argc;                  ///< number of command arguments
  char *nextcmd;                ///< next command (NULL if none)
  char *cmd;                    ///< the name of the command (except for :make)
  char **cmdlinep;              ///< pointer to pointer of allocated cmdline
  cmdidx_T cmdidx;              ///< the index for the command
  uint32_t argt;                ///< flags for the command
  int skip;                     ///< don't execute the command, only parse it
  int forceit;                  ///< true if ! present
  int addr_count;               ///< the number of addresses given
  linenr_T line1;               ///< the first line number
  linenr_T line2;               ///< the second line number or count
  cmd_addr_T addr_type;         ///< type of the count/range
  int flags;                    ///< extra flags after count: EXFLAG_
  char *do_ecmd_cmd;            ///< +command arg to be used in edited file
  linenr_T do_ecmd_lnum;        ///< the line number in an edited file
  int append;                   ///< true with ":w >>file" command
  int usefilter;                ///< true with ":w !command" and ":r!command"
  int amount;                   ///< number of '>' or '<' for shift command
  int regname;                  ///< register name (NUL if none)
  int force_bin;                ///< 0, FORCE_BIN or FORCE_NOBIN
  int read_edit;                ///< ++edit argument
  int mkdir_p;                  ///< ++p argument
  int force_ff;                 ///< ++ff= argument (first char of argument)
  int force_enc;                ///< ++enc= argument (index in cmd[])
  int bad_char;                 ///< BAD_KEEP, BAD_DROP or replacement byte
  int useridx;                  ///< user command index
  char *errmsg;                 ///< returned error message
  LineGetter getline;           ///< Function used to get the next line
  void *cookie;                 ///< argument for getline()
  cstack_T *cstack;             ///< condition stack for ":if" etc.
};

/// Check for '|' to separate commands and '"' to start comments.
void separate_nextcmd(exarg *eap)
{
  char *p = skip_grep_pat(eap);

  for (; *p; MB_PTR_ADV(p)) {
    if (*p == Ctrl_V) {
      if (eap->argt & (EX_CTRLV | EX_XFILE)) {
        p++;  // skip CTRL-V and next char
      } else {
        // remove CTRL-V and skip next char
        STRMOVE(p, p + 1);
      }
      if (*p == NUL) {  // stop at NUL after CTRL-V
        break;
      }
    } else if (p[0] == '`' && p[1] == '=' && (eap->argt & EX_XFILE)) {
      // Skip over `=expr` when wildcards are expanded.
      p += 2;
      (void)skip_expr(&p);
      if (*p == NUL) {  // stop at NUL after CTRL-V
        break;
      }
    } else if (
               // Check for '"': start of comment or '|': next command */
               // :@" does not start a comment!
               // :redir @" doesn't either.
               (*p == '"'
                && !(eap->argt & EX_NOTRLCOM)
                && (eap->cmdidx != CMD_at || p != eap->arg)
                && (eap->cmdidx != CMD_redir
                    || p != eap->arg + 1 || p[-1] != '@')) || *p == '|' || *p == '\n') {
      // We remove the '\' before the '|', unless EX_CTRLV is used
      // AND 'b' is present in 'cpoptions'.
      if ((vim_strchr(p_cpo, CPO_BAR) == NULL
           || !(eap->argt & EX_CTRLV)) && *(p - 1) == '\\') {
        STRMOVE(p - 1, p);  // remove the '\'
        p--;
      } else {
        eap->nextcmd = check_nextcmd(p);
        *p = NUL;
        break;
      }
    }
  }

  if (!(eap->argt & EX_NOTRLCOM)) {  // remove trailing spaces
    del_trailing_spaces(eap->arg);
  }
}
