#pragma once

#include <cstddef>
#include <cinttypes>

// When adding an Ex command:
// 1. Add an entry to the table in src/nvim/ex_cmds.lua.  Keep it sorted on the
//    shortest version of the command name that works.  If it doesn't start with
//    a lower case letter, add it at the end.
//
//    Each table entry is a table with the following keys:
//
//      Key     | Description
//      ------- | -------------------------------------------------------------
//      command | Name of the command. Required.
//      enum    | Name of the enum entry. If not set defaults to CMD_{command}.
//      flags   | A set of the flags from below list joined by bitwise or.
//      func    | Name of the function containing the implementation.
//
//    Referenced function should be either non-static one or defined in
//    ex_docmd.c and be coercible to ex_func_T type from below.
//
//    All keys not described in the above table are reserved for future use.
//
// 2. Add a "case: CMD_xxx" in the big switch in ex_docmd.c.
// 3. Add an entry in the index for Ex commands at ":help ex-cmd-index".
// 4. Add documentation in ../doc/xxx.txt.  Add a tag for both the short and
//    long name of the command.

#define EX_RANGE           0x001u  // allow a linespecs
#define EX_BANG            0x002u  // allow a ! after the command name
#define EX_EXTRA           0x004u  // allow extra args after command name
#define EX_XFILE           0x008u  // expand wildcards in extra part
#define EX_NOSPC           0x010u  // no spaces allowed in the extra part
#define EX_DFLALL          0x020u  // default file range is 1,$
#define EX_WHOLEFOLD       0x040u  // extend range to include whole fold also
                                   // when less than two numbers given
#define EX_NEEDARG         0x080u  // argument required
#define EX_TRLBAR          0x100u  // check for trailing vertical bar
#define EX_REGSTR          0x200u  // allow "x for register designation
#define EX_COUNT           0x400u  // allow count in argument, after command
#define EX_NOTRLCOM        0x800u  // no trailing comment allowed
#define EX_ZEROR          0x1000u  // zero line number allowed
#define EX_CTRLV          0x2000u  // do not remove CTRL-V from argument
#define EX_CMDARG         0x4000u  // allow "+command" argument
#define EX_BUFNAME        0x8000u  // accepts buffer name
#define EX_BUFUNL        0x10000u  // accepts unlisted buffer too
#define EX_ARGOPT        0x20000u  // allow "++opt=val" argument
#define EX_SBOXOK        0x40000u  // allowed in the sandbox
#define EX_CMDWIN        0x80000u  // allowed in cmdline window
#define EX_MODIFY       0x100000u  // forbidden in non-'modifiable' buffer
#define EX_FLAGS        0x200000u  // allow flags after count in argument
#define EX_LOCK_OK     0x1000000u  // command can be executed when textlock is
                                   // set; when missing disallows editing another
                                   // buffer when curbuf->b_ro_locked is set
#define EX_KEEPSCRIPT  0x4000000u  // keep sctx of where command was invoked
#define EX_PREVIEW     0x8000000u  // allow incremental command preview

#define EX_FILES (EX_XFILE | EX_EXTRA)  // multiple extra files allowed
#define EX_FILE1 (EX_FILES | EX_NOSPC)  // 1 file, defaults to current file
#define EX_WORD1 (EX_EXTRA | EX_NOSPC)  // one extra word allowed

// values for cmd_addr_type
typedef enum {
  ADDR_LINES,           // buffer line numbers
  ADDR_WINDOWS,         // window number
  ADDR_ARGUMENTS,       // argument number
  ADDR_LOADED_BUFFERS,  // buffer number of loaded buffer
  ADDR_BUFFERS,         // buffer number
  ADDR_TABS,            // tab page number
  ADDR_TABS_RELATIVE,   // Tab page that only relative
  ADDR_QUICKFIX_VALID,  // quickfix list valid entry number
  ADDR_QUICKFIX,        // quickfix list entry number
  ADDR_UNSIGNED,        // positive count or zero, defaults to 1
  ADDR_OTHER,           // something else, use line number for '$', '%', etc.
  ADDR_NONE,            // no range used
} cmd_addr_T;

typedef struct exarg exarg_T;

// behavior for bad character, "++bad=" argument
#define BAD_REPLACE     '?'     // replace it with '?' (default)
#define BAD_KEEP        (-1)    // leave it
#define BAD_DROP        (-2)    // erase it

typedef void* (*ex_func_T)(void* user);

/// Structure for command definition.
typedef struct cmdname {
  const char* cmd_name;                   ///< Name of the command.
  ex_func_T cmd_func;                     ///< Function creator
  unsigned cmd_argt;                      ///< Relevant flags from the declared above.
  cmd_addr_T cmd_addr_type;               ///< Flag for address type.
} CommandDefinition;

/// Arguments used for Ex commands.
struct exarg {
  char *arg;                    ///< argument of the command
  char **args;                  ///< starting position of command arguments
  size_t *arglens;              ///< length of command arguments
  size_t argc;                  ///< number of command arguments
  char *nextcmd;                ///< next command (NULL if none)
  char *cmd;                    ///< the name of the command (except for :make)
  char **cmdlinep;              ///< pointer to pointer of allocated cmdline
  int cmdidx;                   ///< the index for the command
  unsigned argt;                ///< flags for the command
  int skip;                     ///< don't execute the command, only parse it
  int forceit;                  ///< true if ! present
  int addr_count;               ///< the number of addresses given
  int line1;                    ///< the first line number
  int line2;                    ///< the second line number or count
  cmd_addr_T addr_type;         ///< type of the count/range
  int flags;                    ///< extra flags after count: EXFLAG_
  char *do_ecmd_cmd;            ///< +command arg to be used in edited file
  int do_ecmd_lnum;             ///< the line number in an edited file
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
  void *cookie;                 ///< argument for getline()
};

#define FORCE_BIN 1             // ":edit ++bin file"
#define FORCE_NOBIN 2           // ":edit ++nobin file"

// Values for "flags"
#define EXFLAG_LIST     0x01    // 'l': list
#define EXFLAG_NR       0x02    // '#': number
#define EXFLAG_PRINT    0x04    // 'p': print

enum {
  CMOD_SANDBOX      = 0x0001,  ///< ":sandbox"
  CMOD_SILENT       = 0x0002,  ///< ":silent"
  CMOD_ERRSILENT    = 0x0004,  ///< ":silent!"
  CMOD_UNSILENT     = 0x0008,  ///< ":unsilent"
  CMOD_NOAUTOCMD    = 0x0010,  ///< ":noautocmd"
  CMOD_HIDE         = 0x0020,  ///< ":hide"
  CMOD_BROWSE       = 0x0040,  ///< ":browse" - invoke file dialog
  CMOD_CONFIRM      = 0x0080,  ///< ":confirm" - invoke yes/no dialog
  CMOD_KEEPALT      = 0x0100,  ///< ":keepalt"
  CMOD_KEEPMARKS    = 0x0200,  ///< ":keepmarks"
  CMOD_KEEPJUMPS    = 0x0400,  ///< ":keepjumps"
  CMOD_LOCKMARKS    = 0x0800,  ///< ":lockmarks"
  CMOD_KEEPPATTERNS = 0x1000,  ///< ":keeppatterns"
  CMOD_NOSWAPFILE   = 0x2000,  ///< ":noswapfile"
};

// arguments for win_split()
#define WSP_ROOM        0x01    // require enough room
#define WSP_VERT        0x02    // split/equalize vertically
#define WSP_HOR         0x04    // equalize horizontally
#define WSP_TOP         0x08    // window at top-left of shell
#define WSP_BOT         0x10    // window at bottom-right of shell
#define WSP_HELP        0x20    // creating the help window
#define WSP_BELOW       0x40    // put new window below/right
#define WSP_ABOVE       0x80    // put new window above/left
#define WSP_NEWLOC      0x100   // don't copy location list

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


extern CommandDefinition cmdnames[549];

extern const uint16_t cmdidxs1[26];
extern const uint8_t cmdidxs2[26][26];
