#include "ExCmdsDefs.hpp"
#include "ExCmdsEnum.hpp"

#include <cinttypes>

CommandDefinition cmdnames[549] = {
  [CMD_append] = {
    .cmd_name = "append",
    .cmd_func = nullptr,
    .cmd_argt = 18354435L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_abbreviate] = {
    .cmd_name = "abbreviate",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_abclear] = {
    .cmd_name = "abclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_aboveleft] = {
    .cmd_name = "aboveleft",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_all] = {
    .cmd_name = "all",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_amenu] = {
    .cmd_name = "amenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_anoremenu] = {
    .cmd_name = "anoremenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_args] = {
    .cmd_name = "args",
    .cmd_func = nullptr,
    .cmd_argt = 147726L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_argadd] = {
    .cmd_name = "argadd",
    .cmd_func = nullptr,
    .cmd_argt = 4367L,
    .cmd_addr_type = ADDR_ARGUMENTS
  },
  [CMD_argdelete] = {
    .cmd_name = "argdelete",
    .cmd_func = nullptr,
    .cmd_argt = 271L,
    .cmd_addr_type = ADDR_ARGUMENTS
  },
  [CMD_argdo] = {
    .cmd_name = "argdo",
    .cmd_func = nullptr,
    .cmd_argt = 2215L,
    .cmd_addr_type = ADDR_ARGUMENTS
  },
  [CMD_argdedupe] = {
    .cmd_name = "argdedupe",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_argedit] = {
    .cmd_name = "argedit",
    .cmd_func = nullptr,
    .cmd_argt = 151951L,
    .cmd_addr_type = ADDR_ARGUMENTS
  },
  [CMD_argglobal] = {
    .cmd_name = "argglobal",
    .cmd_func = nullptr,
    .cmd_argt = 147726L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_arglocal] = {
    .cmd_name = "arglocal",
    .cmd_func = nullptr,
    .cmd_argt = 147726L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_argument] = {
    .cmd_name = "argument",
    .cmd_func = nullptr,
    .cmd_argt = 148743L,
    .cmd_addr_type = ADDR_ARGUMENTS
  },
  [CMD_ascii] = {
    .cmd_name = "ascii",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_autocmd] = {
    .cmd_name = "autocmd",
    .cmd_func = nullptr,
    .cmd_argt = 17311750L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_augroup] = {
    .cmd_name = "augroup",
    .cmd_func = nullptr,
    .cmd_argt = 17301782L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_aunmenu] = {
    .cmd_name = "aunmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_buffer] = {
    .cmd_name = "buffer",
    .cmd_func = nullptr,
    .cmd_argt = 115975L,
    .cmd_addr_type = ADDR_BUFFERS
  },
  [CMD_bNext] = {
    .cmd_name = "bNext",
    .cmd_func = nullptr,
    .cmd_argt = 17667L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_ball] = {
    .cmd_name = "ball",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_badd] = {
    .cmd_name = "badd",
    .cmd_func = nullptr,
    .cmd_argt = 17318300L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_balt] = {
    .cmd_name = "balt",
    .cmd_func = nullptr,
    .cmd_argt = 17318300L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_bdelete] = {
    .cmd_name = "bdelete",
    .cmd_func = nullptr,
    .cmd_argt = 34055L,
    .cmd_addr_type = ADDR_BUFFERS
  },
  [CMD_behave] = {
    .cmd_name = "behave",
    .cmd_func = nullptr,
    .cmd_argt = 17301910L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_belowright] = {
    .cmd_name = "belowright",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_bfirst] = {
    .cmd_name = "bfirst",
    .cmd_func = nullptr,
    .cmd_argt = 16643L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_blast] = {
    .cmd_name = "blast",
    .cmd_func = nullptr,
    .cmd_argt = 16643L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_bmodified] = {
    .cmd_name = "bmodified",
    .cmd_func = nullptr,
    .cmd_argt = 17667L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_bnext] = {
    .cmd_name = "bnext",
    .cmd_func = nullptr,
    .cmd_argt = 17667L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_botright] = {
    .cmd_name = "botright",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_bprevious] = {
    .cmd_name = "bprevious",
    .cmd_func = nullptr,
    .cmd_argt = 17667L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_brewind] = {
    .cmd_name = "brewind",
    .cmd_func = nullptr,
    .cmd_argt = 16643L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_break] = {
    .cmd_name = "break",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_breakadd] = {
    .cmd_name = "breakadd",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_breakdel] = {
    .cmd_name = "breakdel",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_breaklist] = {
    .cmd_name = "breaklist",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_browse] = {
    .cmd_name = "browse",
    .cmd_func = nullptr,
    .cmd_argt = 17303684L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_buffers] = {
    .cmd_name = "buffers",
    .cmd_func = nullptr,
    .cmd_argt = 17301766L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_bufdo] = {
    .cmd_name = "bufdo",
    .cmd_func = nullptr,
    .cmd_argt = 2215L,
    .cmd_addr_type = ADDR_BUFFERS
  },
  [CMD_bunload] = {
    .cmd_name = "bunload",
    .cmd_func = nullptr,
    .cmd_argt = 34055L,
    .cmd_addr_type = ADDR_LOADED_BUFFERS
  },
  [CMD_bwipeout] = {
    .cmd_name = "bwipeout",
    .cmd_func = nullptr,
    .cmd_argt = 99591L,
    .cmd_addr_type = ADDR_BUFFERS
  },
  [CMD_change] = {
    .cmd_name = "change",
    .cmd_func = nullptr,
    .cmd_argt = 18351427L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_cNext] = {
    .cmd_name = "cNext",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_cNfile] = {
    .cmd_name = "cNfile",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_cabbrev] = {
    .cmd_name = "cabbrev",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cabclear] = {
    .cmd_name = "cabclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cabove] = {
    .cmd_name = "cabove",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_caddbuffer] = {
    .cmd_name = "caddbuffer",
    .cmd_func = nullptr,
    .cmd_argt = 277L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_caddexpr] = {
    .cmd_name = "caddexpr",
    .cmd_func = nullptr,
    .cmd_argt = 2196L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_caddfile] = {
    .cmd_name = "caddfile",
    .cmd_func = nullptr,
    .cmd_argt = 284L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cafter] = {
    .cmd_name = "cafter",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_call] = {
    .cmd_name = "call",
    .cmd_func = nullptr,
    .cmd_argt = 17565829L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_catch] = {
    .cmd_name = "catch",
    .cmd_func = nullptr,
    .cmd_argt = 17563652L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cbuffer] = {
    .cmd_name = "cbuffer",
    .cmd_func = nullptr,
    .cmd_argt = 279L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_cbefore] = {
    .cmd_name = "cbefore",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_cbelow] = {
    .cmd_name = "cbelow",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_cbottom] = {
    .cmd_name = "cbottom",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cc] = {
    .cmd_name = "cc",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_QUICKFIX
  },
  [CMD_cclose] = {
    .cmd_name = "cclose",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cd] = {
    .cmd_name = "cd",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cdo] = {
    .cmd_name = "cdo",
    .cmd_func = nullptr,
    .cmd_argt = 2215L,
    .cmd_addr_type = ADDR_QUICKFIX_VALID
  },
  [CMD_center] = {
    .cmd_name = "center",
    .cmd_func = nullptr,
    .cmd_argt = 18350405L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_cexpr] = {
    .cmd_name = "cexpr",
    .cmd_func = nullptr,
    .cmd_argt = 2198L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cfile] = {
    .cmd_name = "cfile",
    .cmd_func = nullptr,
    .cmd_argt = 286L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cfdo] = {
    .cmd_name = "cfdo",
    .cmd_func = nullptr,
    .cmd_argt = 2215L,
    .cmd_addr_type = ADDR_QUICKFIX_VALID
  },
  [CMD_cfirst] = {
    .cmd_name = "cfirst",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_cgetfile] = {
    .cmd_name = "cgetfile",
    .cmd_func = nullptr,
    .cmd_argt = 284L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cgetbuffer] = {
    .cmd_name = "cgetbuffer",
    .cmd_func = nullptr,
    .cmd_argt = 277L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_cgetexpr] = {
    .cmd_name = "cgetexpr",
    .cmd_func = nullptr,
    .cmd_argt = 2196L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_chdir] = {
    .cmd_name = "chdir",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_changes] = {
    .cmd_name = "changes",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_checkhealth] = {
    .cmd_name = "checkhealth",
    .cmd_func = nullptr,
    .cmd_argt = 260L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_checkpath] = {
    .cmd_name = "checkpath",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_checktime] = {
    .cmd_name = "checktime",
    .cmd_func = nullptr,
    .cmd_argt = 34053L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_chistory] = {
    .cmd_name = "chistory",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_clist] = {
    .cmd_name = "clist",
    .cmd_func = nullptr,
    .cmd_argt = 17301766L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_clast] = {
    .cmd_name = "clast",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_close] = {
    .cmd_name = "close",
    .cmd_func = nullptr,
    .cmd_argt = 17302787L,
    .cmd_addr_type = ADDR_WINDOWS
  },
  [CMD_clearjumps] = {
    .cmd_name = "clearjumps",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cmap] = {
    .cmd_name = "cmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cmapclear] = {
    .cmd_name = "cmapclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cmenu] = {
    .cmd_name = "cmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_cnext] = {
    .cmd_name = "cnext",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_cnewer] = {
    .cmd_name = "cnewer",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_cnfile] = {
    .cmd_name = "cnfile",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_cnoremap] = {
    .cmd_name = "cnoremap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cnoreabbrev] = {
    .cmd_name = "cnoreabbrev",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cnoremenu] = {
    .cmd_name = "cnoremenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_copy] = {
    .cmd_name = "copy",
    .cmd_func = nullptr,
    .cmd_argt = 18350405L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_colder] = {
    .cmd_name = "colder",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_colorscheme] = {
    .cmd_name = "colorscheme",
    .cmd_func = nullptr,
    .cmd_argt = 17301780L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_command] = {
    .cmd_name = "command",
    .cmd_func = nullptr,
    .cmd_argt = 17311750L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_comclear] = {
    .cmd_name = "comclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_compiler] = {
    .cmd_name = "compiler",
    .cmd_func = nullptr,
    .cmd_argt = 17301782L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_continue] = {
    .cmd_name = "continue",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_confirm] = {
    .cmd_name = "confirm",
    .cmd_func = nullptr,
    .cmd_argt = 17303684L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_const] = {
    .cmd_name = "const",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_copen] = {
    .cmd_name = "copen",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_cprevious] = {
    .cmd_name = "cprevious",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_cpfile] = {
    .cmd_name = "cpfile",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_cquit] = {
    .cmd_name = "cquit",
    .cmd_func = nullptr,
    .cmd_argt = 5379L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_crewind] = {
    .cmd_name = "crewind",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_cunmap] = {
    .cmd_name = "cunmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cunabbrev] = {
    .cmd_name = "cunabbrev",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cunmenu] = {
    .cmd_name = "cunmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_cwindow] = {
    .cmd_name = "cwindow",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_delete] = {
    .cmd_name = "delete",
    .cmd_func = nullptr,
    .cmd_argt = 18351937L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_delmarks] = {
    .cmd_name = "delmarks",
    .cmd_func = nullptr,
    .cmd_argt = 17301766L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_debug] = {
    .cmd_name = "debug",
    .cmd_func = nullptr,
    .cmd_argt = 17565828L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_debuggreedy] = {
    .cmd_name = "debuggreedy",
    .cmd_func = nullptr,
    .cmd_argt = 17305857L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_delcommand] = {
    .cmd_name = "delcommand",
    .cmd_func = nullptr,
    .cmd_argt = 17301910L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_delfunction] = {
    .cmd_name = "delfunction",
    .cmd_func = nullptr,
    .cmd_argt = 17301654L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_display] = {
    .cmd_name = "display",
    .cmd_func = nullptr,
    .cmd_argt = 17565956L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_diffupdate] = {
    .cmd_name = "diffupdate",
    .cmd_func = nullptr,
    .cmd_argt = 258L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_diffget] = {
    .cmd_name = "diffget",
    .cmd_func = nullptr,
    .cmd_argt = 1048837L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_diffoff] = {
    .cmd_name = "diffoff",
    .cmd_func = nullptr,
    .cmd_argt = 258L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_diffpatch] = {
    .cmd_name = "diffpatch",
    .cmd_func = nullptr,
    .cmd_argt = 1048860L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_diffput] = {
    .cmd_name = "diffput",
    .cmd_func = nullptr,
    .cmd_argt = 261L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_diffsplit] = {
    .cmd_name = "diffsplit",
    .cmd_func = nullptr,
    .cmd_argt = 284L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_diffthis] = {
    .cmd_name = "diffthis",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_digraphs] = {
    .cmd_name = "digraphs",
    .cmd_func = nullptr,
    .cmd_argt = 17301766L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_djump] = {
    .cmd_name = "djump",
    .cmd_func = nullptr,
    .cmd_argt = 103L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_dlist] = {
    .cmd_name = "dlist",
    .cmd_func = nullptr,
    .cmd_argt = 17301607L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_doautocmd] = {
    .cmd_name = "doautocmd",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_doautoall] = {
    .cmd_name = "doautoall",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_drop] = {
    .cmd_name = "drop",
    .cmd_func = nullptr,
    .cmd_argt = 147852L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_dsearch] = {
    .cmd_name = "dsearch",
    .cmd_func = nullptr,
    .cmd_argt = 17301607L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_dsplit] = {
    .cmd_name = "dsplit",
    .cmd_func = nullptr,
    .cmd_argt = 103L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_edit] = {
    .cmd_name = "edit",
    .cmd_func = nullptr,
    .cmd_argt = 147742L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_earlier] = {
    .cmd_name = "earlier",
    .cmd_func = nullptr,
    .cmd_argt = 17301780L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_echo] = {
    .cmd_name = "echo",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_echoerr] = {
    .cmd_name = "echoerr",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_echohl] = {
    .cmd_name = "echohl",
    .cmd_func = nullptr,
    .cmd_argt = 17563908L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_echomsg] = {
    .cmd_name = "echomsg",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_echon] = {
    .cmd_name = "echon",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_else] = {
    .cmd_name = "else",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_elseif] = {
    .cmd_name = "elseif",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_emenu] = {
    .cmd_name = "emenu",
    .cmd_func = nullptr,
    .cmd_argt = 17303941L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_endif] = {
    .cmd_name = "endif",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_endfunction] = {
    .cmd_name = "endfunction",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_endfor] = {
    .cmd_name = "endfor",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_endtry] = {
    .cmd_name = "endtry",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_endwhile] = {
    .cmd_name = "endwhile",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_enew] = {
    .cmd_name = "enew",
    .cmd_func = nullptr,
    .cmd_argt = 258L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_eval] = {
    .cmd_name = "eval",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_ex] = {
    .cmd_name = "ex",
    .cmd_func = nullptr,
    .cmd_argt = 147742L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_execute] = {
    .cmd_name = "execute",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_exit] = {
    .cmd_name = "exit",
    .cmd_func = nullptr,
    .cmd_argt = 17432959L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_exusage] = {
    .cmd_name = "exusage",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_file] = {
    .cmd_name = "file",
    .cmd_func = nullptr,
    .cmd_argt = 4383L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_files] = {
    .cmd_name = "files",
    .cmd_func = nullptr,
    .cmd_argt = 17301766L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_filetype] = {
    .cmd_name = "filetype",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_filter] = {
    .cmd_name = "filter",
    .cmd_func = nullptr,
    .cmd_argt = 2182L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_find] = {
    .cmd_name = "find",
    .cmd_func = nullptr,
    .cmd_argt = 147871L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_finally] = {
    .cmd_name = "finally",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_finish] = {
    .cmd_name = "finish",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_first] = {
    .cmd_name = "first",
    .cmd_func = nullptr,
    .cmd_argt = 147718L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_fold] = {
    .cmd_name = "fold",
    .cmd_func = nullptr,
    .cmd_argt = 17563969L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_foldclose] = {
    .cmd_name = "foldclose",
    .cmd_func = nullptr,
    .cmd_argt = 17563971L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_folddoopen] = {
    .cmd_name = "folddoopen",
    .cmd_func = nullptr,
    .cmd_argt = 2213L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_folddoclosed] = {
    .cmd_name = "folddoclosed",
    .cmd_func = nullptr,
    .cmd_argt = 2213L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_foldopen] = {
    .cmd_name = "foldopen",
    .cmd_func = nullptr,
    .cmd_argt = 17563971L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_for] = {
    .cmd_name = "for",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_function] = {
    .cmd_name = "function",
    .cmd_func = nullptr,
    .cmd_argt = 17563654L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_global] = {
    .cmd_name = "global",
    .cmd_func = nullptr,
    .cmd_argt = 17563751L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_goto] = {
    .cmd_name = "goto",
    .cmd_func = nullptr,
    .cmd_argt = 17564929L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_grep] = {
    .cmd_name = "grep",
    .cmd_func = nullptr,
    .cmd_argt = 2447L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_grepadd] = {
    .cmd_name = "grepadd",
    .cmd_func = nullptr,
    .cmd_argt = 2447L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_gui] = {
    .cmd_name = "gui",
    .cmd_func = nullptr,
    .cmd_argt = 17449230L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_gvim] = {
    .cmd_name = "gvim",
    .cmd_func = nullptr,
    .cmd_argt = 17449230L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_help] = {
    .cmd_name = "help",
    .cmd_func = nullptr,
    .cmd_argt = 2054L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_helpclose] = {
    .cmd_name = "helpclose",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_helpgrep] = {
    .cmd_name = "helpgrep",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_helptags] = {
    .cmd_name = "helptags",
    .cmd_func = nullptr,
    .cmd_argt = 17301900L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_highlight] = {
    .cmd_name = "highlight",
    .cmd_func = nullptr,
    .cmd_argt = 17563910L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_hide] = {
    .cmd_name = "hide",
    .cmd_func = nullptr,
    .cmd_argt = 1287L,
    .cmd_addr_type = ADDR_WINDOWS
  },
  [CMD_history] = {
    .cmd_name = "history",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_horizontal] = {
    .cmd_name = "horizontal",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_insert] = {
    .cmd_name = "insert",
    .cmd_func = nullptr,
    .cmd_argt = 18350339L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_iabbrev] = {
    .cmd_name = "iabbrev",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_iabclear] = {
    .cmd_name = "iabclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_if] = {
    .cmd_name = "if",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_ijump] = {
    .cmd_name = "ijump",
    .cmd_func = nullptr,
    .cmd_argt = 103L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_ilist] = {
    .cmd_name = "ilist",
    .cmd_func = nullptr,
    .cmd_argt = 17301607L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_imap] = {
    .cmd_name = "imap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_imapclear] = {
    .cmd_name = "imapclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_imenu] = {
    .cmd_name = "imenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_inoremap] = {
    .cmd_name = "inoremap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_inoreabbrev] = {
    .cmd_name = "inoreabbrev",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_inoremenu] = {
    .cmd_name = "inoremenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_intro] = {
    .cmd_name = "intro",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_isearch] = {
    .cmd_name = "isearch",
    .cmd_func = nullptr,
    .cmd_argt = 17301607L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_isplit] = {
    .cmd_name = "isplit",
    .cmd_func = nullptr,
    .cmd_argt = 103L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_iunmap] = {
    .cmd_name = "iunmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_iunabbrev] = {
    .cmd_name = "iunabbrev",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_iunmenu] = {
    .cmd_name = "iunmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_join] = {
    .cmd_name = "join",
    .cmd_func = nullptr,
    .cmd_argt = 20448579L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_jumps] = {
    .cmd_name = "jumps",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_k] = {
    .cmd_name = "k",
    .cmd_func = nullptr,
    .cmd_argt = 17563925L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_keepmarks] = {
    .cmd_name = "keepmarks",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_keepjumps] = {
    .cmd_name = "keepjumps",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_keeppatterns] = {
    .cmd_name = "keeppatterns",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_keepalt] = {
    .cmd_name = "keepalt",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_list] = {
    .cmd_name = "list",
    .cmd_func = nullptr,
    .cmd_argt = 19400001L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_lNext] = {
    .cmd_name = "lNext",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_lNfile] = {
    .cmd_name = "lNfile",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_last] = {
    .cmd_name = "last",
    .cmd_func = nullptr,
    .cmd_argt = 147718L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_labove] = {
    .cmd_name = "labove",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_language] = {
    .cmd_name = "language",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_laddexpr] = {
    .cmd_name = "laddexpr",
    .cmd_func = nullptr,
    .cmd_argt = 2196L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_laddbuffer] = {
    .cmd_name = "laddbuffer",
    .cmd_func = nullptr,
    .cmd_argt = 277L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_laddfile] = {
    .cmd_name = "laddfile",
    .cmd_func = nullptr,
    .cmd_argt = 284L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lafter] = {
    .cmd_name = "lafter",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_later] = {
    .cmd_name = "later",
    .cmd_func = nullptr,
    .cmd_argt = 17301780L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lbuffer] = {
    .cmd_name = "lbuffer",
    .cmd_func = nullptr,
    .cmd_argt = 279L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_lbefore] = {
    .cmd_name = "lbefore",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_lbelow] = {
    .cmd_name = "lbelow",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_lbottom] = {
    .cmd_name = "lbottom",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lcd] = {
    .cmd_name = "lcd",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lchdir] = {
    .cmd_name = "lchdir",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lclose] = {
    .cmd_name = "lclose",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_ldo] = {
    .cmd_name = "ldo",
    .cmd_func = nullptr,
    .cmd_argt = 2215L,
    .cmd_addr_type = ADDR_QUICKFIX_VALID
  },
  [CMD_left] = {
    .cmd_name = "left",
    .cmd_func = nullptr,
    .cmd_argt = 18350405L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_leftabove] = {
    .cmd_name = "leftabove",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_let] = {
    .cmd_name = "let",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lexpr] = {
    .cmd_name = "lexpr",
    .cmd_func = nullptr,
    .cmd_argt = 2198L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lfile] = {
    .cmd_name = "lfile",
    .cmd_func = nullptr,
    .cmd_argt = 286L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lfdo] = {
    .cmd_name = "lfdo",
    .cmd_func = nullptr,
    .cmd_argt = 2215L,
    .cmd_addr_type = ADDR_QUICKFIX_VALID
  },
  [CMD_lfirst] = {
    .cmd_name = "lfirst",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_lgetfile] = {
    .cmd_name = "lgetfile",
    .cmd_func = nullptr,
    .cmd_argt = 284L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lgetbuffer] = {
    .cmd_name = "lgetbuffer",
    .cmd_func = nullptr,
    .cmd_argt = 277L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_lgetexpr] = {
    .cmd_name = "lgetexpr",
    .cmd_func = nullptr,
    .cmd_argt = 2196L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lgrep] = {
    .cmd_name = "lgrep",
    .cmd_func = nullptr,
    .cmd_argt = 2447L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_lgrepadd] = {
    .cmd_name = "lgrepadd",
    .cmd_func = nullptr,
    .cmd_argt = 2447L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_lhelpgrep] = {
    .cmd_name = "lhelpgrep",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lhistory] = {
    .cmd_name = "lhistory",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_ll] = {
    .cmd_name = "ll",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_QUICKFIX
  },
  [CMD_llast] = {
    .cmd_name = "llast",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_llist] = {
    .cmd_name = "llist",
    .cmd_func = nullptr,
    .cmd_argt = 17301766L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lmap] = {
    .cmd_name = "lmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lmapclear] = {
    .cmd_name = "lmapclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lmake] = {
    .cmd_name = "lmake",
    .cmd_func = nullptr,
    .cmd_argt = 2318L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lnoremap] = {
    .cmd_name = "lnoremap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lnext] = {
    .cmd_name = "lnext",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_lnewer] = {
    .cmd_name = "lnewer",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_lnfile] = {
    .cmd_name = "lnfile",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_loadview] = {
    .cmd_name = "loadview",
    .cmd_func = nullptr,
    .cmd_argt = 284L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_loadkeymap] = {
    .cmd_name = "loadkeymap",
    .cmd_func = nullptr,
    .cmd_argt = 17301504L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lockmarks] = {
    .cmd_name = "lockmarks",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lockvar] = {
    .cmd_name = "lockvar",
    .cmd_func = nullptr,
    .cmd_argt = 17563782L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lolder] = {
    .cmd_name = "lolder",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_lopen] = {
    .cmd_name = "lopen",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_lprevious] = {
    .cmd_name = "lprevious",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_lpfile] = {
    .cmd_name = "lpfile",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_lrewind] = {
    .cmd_name = "lrewind",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_UNSIGNED
  },
  [CMD_ltag] = {
    .cmd_name = "ltag",
    .cmd_func = nullptr,
    .cmd_argt = 278L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lunmap] = {
    .cmd_name = "lunmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_lua] = {
    .cmd_name = "lua",
    .cmd_func = nullptr,
    .cmd_argt = 17301637L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_luado] = {
    .cmd_name = "luado",
    .cmd_func = nullptr,
    .cmd_argt = 17301669L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_luafile] = {
    .cmd_name = "luafile",
    .cmd_func = nullptr,
    .cmd_argt = 17301661L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_lvimgrep] = {
    .cmd_name = "lvimgrep",
    .cmd_func = nullptr,
    .cmd_argt = 16779663L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_lvimgrepadd] = {
    .cmd_name = "lvimgrepadd",
    .cmd_func = nullptr,
    .cmd_argt = 16779663L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_lwindow] = {
    .cmd_name = "lwindow",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_ls] = {
    .cmd_name = "ls",
    .cmd_func = nullptr,
    .cmd_argt = 17301766L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_move] = {
    .cmd_name = "move",
    .cmd_func = nullptr,
    .cmd_argt = 18350405L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_mark] = {
    .cmd_name = "mark",
    .cmd_func = nullptr,
    .cmd_argt = 17563925L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_make] = {
    .cmd_name = "make",
    .cmd_func = nullptr,
    .cmd_argt = 2318L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_map] = {
    .cmd_name = "map",
    .cmd_func = nullptr,
    .cmd_argt = 17312006L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_mapclear] = {
    .cmd_name = "mapclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301766L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_marks] = {
    .cmd_name = "marks",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_match] = {
    .cmd_name = "match",
    .cmd_func = nullptr,
    .cmd_argt = 17301509L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_menu] = {
    .cmd_name = "menu",
    .cmd_func = nullptr,
    .cmd_argt = 17316103L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_menutranslate] = {
    .cmd_name = "menutranslate",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_messages] = {
    .cmd_name = "messages",
    .cmd_func = nullptr,
    .cmd_argt = 17301765L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_mkexrc] = {
    .cmd_name = "mkexrc",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_mksession] = {
    .cmd_name = "mksession",
    .cmd_func = nullptr,
    .cmd_argt = 286L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_mkspell] = {
    .cmd_name = "mkspell",
    .cmd_func = nullptr,
    .cmd_argt = 2446L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_mkvimrc] = {
    .cmd_name = "mkvimrc",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_mkview] = {
    .cmd_name = "mkview",
    .cmd_func = nullptr,
    .cmd_argt = 286L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_mode] = {
    .cmd_name = "mode",
    .cmd_func = nullptr,
    .cmd_argt = 17301780L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_mzscheme] = {
    .cmd_name = "mzscheme",
    .cmd_func = nullptr,
    .cmd_argt = 17563813L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_mzfile] = {
    .cmd_name = "mzfile",
    .cmd_func = nullptr,
    .cmd_argt = 17301661L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_next] = {
    .cmd_name = "next",
    .cmd_func = nullptr,
    .cmd_argt = 147727L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_new] = {
    .cmd_name = "new",
    .cmd_func = nullptr,
    .cmd_argt = 147743L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_nmap] = {
    .cmd_name = "nmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_nmapclear] = {
    .cmd_name = "nmapclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_nmenu] = {
    .cmd_name = "nmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_nnoremap] = {
    .cmd_name = "nnoremap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_nnoremenu] = {
    .cmd_name = "nnoremenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_noremap] = {
    .cmd_name = "noremap",
    .cmd_func = nullptr,
    .cmd_argt = 17312006L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_noautocmd] = {
    .cmd_name = "noautocmd",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_nohlsearch] = {
    .cmd_name = "nohlsearch",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_noreabbrev] = {
    .cmd_name = "noreabbrev",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_noremenu] = {
    .cmd_name = "noremenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316103L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_noswapfile] = {
    .cmd_name = "noswapfile",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_normal] = {
    .cmd_name = "normal",
    .cmd_func = nullptr,
    .cmd_argt = 17574023L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_number] = {
    .cmd_name = "number",
    .cmd_func = nullptr,
    .cmd_argt = 19400001L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_nunmap] = {
    .cmd_name = "nunmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_nunmenu] = {
    .cmd_name = "nunmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_oldfiles] = {
    .cmd_name = "oldfiles",
    .cmd_func = nullptr,
    .cmd_argt = 17563906L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_omap] = {
    .cmd_name = "omap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_omapclear] = {
    .cmd_name = "omapclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_omenu] = {
    .cmd_name = "omenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_only] = {
    .cmd_name = "only",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_WINDOWS
  },
  [CMD_onoremap] = {
    .cmd_name = "onoremap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_onoremenu] = {
    .cmd_name = "onoremenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_options] = {
    .cmd_name = "options",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_ounmap] = {
    .cmd_name = "ounmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_ounmenu] = {
    .cmd_name = "ounmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_ownsyntax] = {
    .cmd_name = "ownsyntax",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_print] = {
    .cmd_name = "print",
    .cmd_func = nullptr,
    .cmd_argt = 19662145L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_packadd] = {
    .cmd_name = "packadd",
    .cmd_func = nullptr,
    .cmd_argt = 17564062L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_packloadall] = {
    .cmd_name = "packloadall",
    .cmd_func = nullptr,
    .cmd_argt = 17563906L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_pclose] = {
    .cmd_name = "pclose",
    .cmd_func = nullptr,
    .cmd_argt = 258L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_perl] = {
    .cmd_name = "perl",
    .cmd_func = nullptr,
    .cmd_argt = 17563813L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_perldo] = {
    .cmd_name = "perldo",
    .cmd_func = nullptr,
    .cmd_argt = 17301669L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_perlfile] = {
    .cmd_name = "perlfile",
    .cmd_func = nullptr,
    .cmd_argt = 17301661L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_pedit] = {
    .cmd_name = "pedit",
    .cmd_func = nullptr,
    .cmd_argt = 147742L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_pop] = {
    .cmd_name = "pop",
    .cmd_func = nullptr,
    .cmd_argt = 5379L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_popup] = {
    .cmd_name = "popup",
    .cmd_func = nullptr,
    .cmd_argt = 17303942L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_ppop] = {
    .cmd_name = "ppop",
    .cmd_func = nullptr,
    .cmd_argt = 5379L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_preserve] = {
    .cmd_name = "preserve",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_previous] = {
    .cmd_name = "previous",
    .cmd_func = nullptr,
    .cmd_argt = 148743L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_profile] = {
    .cmd_name = "profile",
    .cmd_func = nullptr,
    .cmd_argt = 17301766L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_profdel] = {
    .cmd_name = "profdel",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_psearch] = {
    .cmd_name = "psearch",
    .cmd_func = nullptr,
    .cmd_argt = 103L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_ptag] = {
    .cmd_name = "ptag",
    .cmd_func = nullptr,
    .cmd_argt = 4375L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_ptNext] = {
    .cmd_name = "ptNext",
    .cmd_func = nullptr,
    .cmd_argt = 4355L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_ptfirst] = {
    .cmd_name = "ptfirst",
    .cmd_func = nullptr,
    .cmd_argt = 4355L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_ptjump] = {
    .cmd_name = "ptjump",
    .cmd_func = nullptr,
    .cmd_argt = 278L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_ptlast] = {
    .cmd_name = "ptlast",
    .cmd_func = nullptr,
    .cmd_argt = 258L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_ptnext] = {
    .cmd_name = "ptnext",
    .cmd_func = nullptr,
    .cmd_argt = 4355L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_ptprevious] = {
    .cmd_name = "ptprevious",
    .cmd_func = nullptr,
    .cmd_argt = 4355L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_ptrewind] = {
    .cmd_name = "ptrewind",
    .cmd_func = nullptr,
    .cmd_argt = 4355L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_ptselect] = {
    .cmd_name = "ptselect",
    .cmd_func = nullptr,
    .cmd_argt = 278L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_put] = {
    .cmd_name = "put",
    .cmd_func = nullptr,
    .cmd_argt = 18355011L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_pwd] = {
    .cmd_name = "pwd",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_python] = {
    .cmd_name = "python",
    .cmd_func = nullptr,
    .cmd_argt = 17301637L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_pydo] = {
    .cmd_name = "pydo",
    .cmd_func = nullptr,
    .cmd_argt = 17301669L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_pyfile] = {
    .cmd_name = "pyfile",
    .cmd_func = nullptr,
    .cmd_argt = 17301661L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_py3] = {
    .cmd_name = "py3",
    .cmd_func = nullptr,
    .cmd_argt = 17301637L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_py3do] = {
    .cmd_name = "py3do",
    .cmd_func = nullptr,
    .cmd_argt = 17301669L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_python3] = {
    .cmd_name = "python3",
    .cmd_func = nullptr,
    .cmd_argt = 17301637L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_py3file] = {
    .cmd_name = "py3file",
    .cmd_func = nullptr,
    .cmd_argt = 17301661L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_pyx] = {
    .cmd_name = "pyx",
    .cmd_func = nullptr,
    .cmd_argt = 17301637L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_pyxdo] = {
    .cmd_name = "pyxdo",
    .cmd_func = nullptr,
    .cmd_argt = 17301669L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_pythonx] = {
    .cmd_name = "pythonx",
    .cmd_func = nullptr,
    .cmd_argt = 17301637L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_pyxfile] = {
    .cmd_name = "pyxfile",
    .cmd_func = nullptr,
    .cmd_argt = 17301661L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_quit] = {
    .cmd_name = "quit",
    .cmd_func = nullptr,
    .cmd_argt = 17302787L,
    .cmd_addr_type = ADDR_WINDOWS
  },
  [CMD_quitall] = {
    .cmd_name = "quitall",
    .cmd_func = nullptr,
    .cmd_argt = 258L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_qall] = {
    .cmd_name = "qall",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_read] = {
    .cmd_name = "read",
    .cmd_func = nullptr,
    .cmd_argt = 18485599L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_recover] = {
    .cmd_name = "recover",
    .cmd_func = nullptr,
    .cmd_argt = 286L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_redo] = {
    .cmd_name = "redo",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_redir] = {
    .cmd_name = "redir",
    .cmd_func = nullptr,
    .cmd_argt = 17301774L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_redraw] = {
    .cmd_name = "redraw",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_redrawstatus] = {
    .cmd_name = "redrawstatus",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_redrawtabline] = {
    .cmd_name = "redrawtabline",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_registers] = {
    .cmd_name = "registers",
    .cmd_func = nullptr,
    .cmd_argt = 17303812L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_resize] = {
    .cmd_name = "resize",
    .cmd_func = nullptr,
    .cmd_argt = 17301781L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_retab] = {
    .cmd_name = "retab",
    .cmd_func = nullptr,
    .cmd_argt = 18350455L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_return] = {
    .cmd_name = "return",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_rewind] = {
    .cmd_name = "rewind",
    .cmd_func = nullptr,
    .cmd_argt = 147718L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_right] = {
    .cmd_name = "right",
    .cmd_func = nullptr,
    .cmd_argt = 18350405L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_rightbelow] = {
    .cmd_name = "rightbelow",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_rshada] = {
    .cmd_name = "rshada",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_runtime] = {
    .cmd_name = "runtime",
    .cmd_func = nullptr,
    .cmd_argt = 17564046L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_rundo] = {
    .cmd_name = "rundo",
    .cmd_func = nullptr,
    .cmd_argt = 156L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_ruby] = {
    .cmd_name = "ruby",
    .cmd_func = nullptr,
    .cmd_argt = 17301637L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_rubydo] = {
    .cmd_name = "rubydo",
    .cmd_func = nullptr,
    .cmd_argt = 17301669L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_rubyfile] = {
    .cmd_name = "rubyfile",
    .cmd_func = nullptr,
    .cmd_argt = 17301661L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_rviminfo] = {
    .cmd_name = "rviminfo",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_substitute] = {
    .cmd_name = "substitute",
    .cmd_func = nullptr,
    .cmd_argt = 151519301L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_sNext] = {
    .cmd_name = "sNext",
    .cmd_func = nullptr,
    .cmd_argt = 148743L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_sargument] = {
    .cmd_name = "sargument",
    .cmd_func = nullptr,
    .cmd_argt = 148743L,
    .cmd_addr_type = ADDR_ARGUMENTS
  },
  [CMD_sall] = {
    .cmd_name = "sall",
    .cmd_func = nullptr,
    .cmd_argt = 1283L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_sandbox] = {
    .cmd_name = "sandbox",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_saveas] = {
    .cmd_name = "saveas",
    .cmd_func = nullptr,
    .cmd_argt = 17432862L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_sbuffer] = {
    .cmd_name = "sbuffer",
    .cmd_func = nullptr,
    .cmd_argt = 115975L,
    .cmd_addr_type = ADDR_BUFFERS
  },
  [CMD_sbNext] = {
    .cmd_name = "sbNext",
    .cmd_func = nullptr,
    .cmd_argt = 17665L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_sball] = {
    .cmd_name = "sball",
    .cmd_func = nullptr,
    .cmd_argt = 17665L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_sbfirst] = {
    .cmd_name = "sbfirst",
    .cmd_func = nullptr,
    .cmd_argt = 16640L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_sblast] = {
    .cmd_name = "sblast",
    .cmd_func = nullptr,
    .cmd_argt = 16640L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_sbmodified] = {
    .cmd_name = "sbmodified",
    .cmd_func = nullptr,
    .cmd_argt = 17665L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_sbnext] = {
    .cmd_name = "sbnext",
    .cmd_func = nullptr,
    .cmd_argt = 17665L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_sbprevious] = {
    .cmd_name = "sbprevious",
    .cmd_func = nullptr,
    .cmd_argt = 17665L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_sbrewind] = {
    .cmd_name = "sbrewind",
    .cmd_func = nullptr,
    .cmd_argt = 16640L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_scriptnames] = {
    .cmd_name = "scriptnames",
    .cmd_func = nullptr,
    .cmd_argt = 17302799L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_scriptencoding] = {
    .cmd_name = "scriptencoding",
    .cmd_func = nullptr,
    .cmd_argt = 17301780L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_set] = {
    .cmd_name = "set",
    .cmd_func = nullptr,
    .cmd_argt = 17563910L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_setfiletype] = {
    .cmd_name = "setfiletype",
    .cmd_func = nullptr,
    .cmd_argt = 17301892L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_setglobal] = {
    .cmd_name = "setglobal",
    .cmd_func = nullptr,
    .cmd_argt = 17563910L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_setlocal] = {
    .cmd_name = "setlocal",
    .cmd_func = nullptr,
    .cmd_argt = 17563910L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_sfind] = {
    .cmd_name = "sfind",
    .cmd_func = nullptr,
    .cmd_argt = 147871L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_sfirst] = {
    .cmd_name = "sfirst",
    .cmd_func = nullptr,
    .cmd_argt = 147718L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_simalt] = {
    .cmd_name = "simalt",
    .cmd_func = nullptr,
    .cmd_argt = 17301908L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_sign] = {
    .cmd_name = "sign",
    .cmd_func = nullptr,
    .cmd_argt = 17301637L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_silent] = {
    .cmd_name = "silent",
    .cmd_func = nullptr,
    .cmd_argt = 17565830L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_sleep] = {
    .cmd_name = "sleep",
    .cmd_func = nullptr,
    .cmd_argt = 17302791L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_slast] = {
    .cmd_name = "slast",
    .cmd_func = nullptr,
    .cmd_argt = 147718L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_smagic] = {
    .cmd_name = "smagic",
    .cmd_func = nullptr,
    .cmd_argt = 151519301L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_smap] = {
    .cmd_name = "smap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_smapclear] = {
    .cmd_name = "smapclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_smenu] = {
    .cmd_name = "smenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_snext] = {
    .cmd_name = "snext",
    .cmd_func = nullptr,
    .cmd_argt = 147727L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_snomagic] = {
    .cmd_name = "snomagic",
    .cmd_func = nullptr,
    .cmd_argt = 151519301L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_snoremap] = {
    .cmd_name = "snoremap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_snoremenu] = {
    .cmd_name = "snoremenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_source] = {
    .cmd_name = "source",
    .cmd_func = nullptr,
    .cmd_argt = 17564031L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_sort] = {
    .cmd_name = "sort",
    .cmd_func = nullptr,
    .cmd_argt = 1050727L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_split] = {
    .cmd_name = "split",
    .cmd_func = nullptr,
    .cmd_argt = 147743L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_spellgood] = {
    .cmd_name = "spellgood",
    .cmd_func = nullptr,
    .cmd_argt = 391L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_spelldump] = {
    .cmd_name = "spelldump",
    .cmd_func = nullptr,
    .cmd_argt = 258L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_spellinfo] = {
    .cmd_name = "spellinfo",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_spellrepall] = {
    .cmd_name = "spellrepall",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_spellrare] = {
    .cmd_name = "spellrare",
    .cmd_func = nullptr,
    .cmd_argt = 391L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_spellundo] = {
    .cmd_name = "spellundo",
    .cmd_func = nullptr,
    .cmd_argt = 391L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_spellwrong] = {
    .cmd_name = "spellwrong",
    .cmd_func = nullptr,
    .cmd_argt = 391L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_sprevious] = {
    .cmd_name = "sprevious",
    .cmd_func = nullptr,
    .cmd_argt = 148743L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_srewind] = {
    .cmd_name = "srewind",
    .cmd_func = nullptr,
    .cmd_argt = 147718L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_stop] = {
    .cmd_name = "stop",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_stag] = {
    .cmd_name = "stag",
    .cmd_func = nullptr,
    .cmd_argt = 4375L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_startinsert] = {
    .cmd_name = "startinsert",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_startgreplace] = {
    .cmd_name = "startgreplace",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_startreplace] = {
    .cmd_name = "startreplace",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_stopinsert] = {
    .cmd_name = "stopinsert",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_stjump] = {
    .cmd_name = "stjump",
    .cmd_func = nullptr,
    .cmd_argt = 278L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_stselect] = {
    .cmd_name = "stselect",
    .cmd_func = nullptr,
    .cmd_argt = 278L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_sunhide] = {
    .cmd_name = "sunhide",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_sunmap] = {
    .cmd_name = "sunmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_sunmenu] = {
    .cmd_name = "sunmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_suspend] = {
    .cmd_name = "suspend",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_sview] = {
    .cmd_name = "sview",
    .cmd_func = nullptr,
    .cmd_argt = 147743L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_swapname] = {
    .cmd_name = "swapname",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_syntax] = {
    .cmd_name = "syntax",
    .cmd_func = nullptr,
    .cmd_argt = 17303556L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_syntime] = {
    .cmd_name = "syntime",
    .cmd_func = nullptr,
    .cmd_argt = 17301908L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_syncbind] = {
    .cmd_name = "syncbind",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_t] = {
    .cmd_name = "t",
    .cmd_func = nullptr,
    .cmd_argt = 18350405L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_tcd] = {
    .cmd_name = "tcd",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tchdir] = {
    .cmd_name = "tchdir",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tNext] = {
    .cmd_name = "tNext",
    .cmd_func = nullptr,
    .cmd_argt = 4355L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_tag] = {
    .cmd_name = "tag",
    .cmd_func = nullptr,
    .cmd_argt = 4375L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_tags] = {
    .cmd_name = "tags",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tab] = {
    .cmd_name = "tab",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tabclose] = {
    .cmd_name = "tabclose",
    .cmd_func = nullptr,
    .cmd_argt = 17305879L,
    .cmd_addr_type = ADDR_TABS
  },
  [CMD_tabdo] = {
    .cmd_name = "tabdo",
    .cmd_func = nullptr,
    .cmd_argt = 2213L,
    .cmd_addr_type = ADDR_TABS
  },
  [CMD_tabedit] = {
    .cmd_name = "tabedit",
    .cmd_func = nullptr,
    .cmd_argt = 151839L,
    .cmd_addr_type = ADDR_TABS
  },
  [CMD_tabfind] = {
    .cmd_name = "tabfind",
    .cmd_func = nullptr,
    .cmd_argt = 151967L,
    .cmd_addr_type = ADDR_TABS
  },
  [CMD_tabfirst] = {
    .cmd_name = "tabfirst",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tabmove] = {
    .cmd_name = "tabmove",
    .cmd_func = nullptr,
    .cmd_argt = 4373L,
    .cmd_addr_type = ADDR_TABS
  },
  [CMD_tablast] = {
    .cmd_name = "tablast",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tabnext] = {
    .cmd_name = "tabnext",
    .cmd_func = nullptr,
    .cmd_argt = 4373L,
    .cmd_addr_type = ADDR_TABS
  },
  [CMD_tabnew] = {
    .cmd_name = "tabnew",
    .cmd_func = nullptr,
    .cmd_argt = 151839L,
    .cmd_addr_type = ADDR_TABS
  },
  [CMD_tabonly] = {
    .cmd_name = "tabonly",
    .cmd_func = nullptr,
    .cmd_argt = 17305879L,
    .cmd_addr_type = ADDR_TABS
  },
  [CMD_tabprevious] = {
    .cmd_name = "tabprevious",
    .cmd_func = nullptr,
    .cmd_argt = 4373L,
    .cmd_addr_type = ADDR_TABS_RELATIVE
  },
  [CMD_tabNext] = {
    .cmd_name = "tabNext",
    .cmd_func = nullptr,
    .cmd_argt = 4373L,
    .cmd_addr_type = ADDR_TABS_RELATIVE
  },
  [CMD_tabrewind] = {
    .cmd_name = "tabrewind",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tabs] = {
    .cmd_name = "tabs",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tcl] = {
    .cmd_name = "tcl",
    .cmd_func = nullptr,
    .cmd_argt = 17301637L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_tcldo] = {
    .cmd_name = "tcldo",
    .cmd_func = nullptr,
    .cmd_argt = 17301669L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_tclfile] = {
    .cmd_name = "tclfile",
    .cmd_func = nullptr,
    .cmd_argt = 17301661L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_terminal] = {
    .cmd_name = "terminal",
    .cmd_func = nullptr,
    .cmd_argt = 17301518L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tfirst] = {
    .cmd_name = "tfirst",
    .cmd_func = nullptr,
    .cmd_argt = 4355L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_throw] = {
    .cmd_name = "throw",
    .cmd_func = nullptr,
    .cmd_argt = 17563780L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tjump] = {
    .cmd_name = "tjump",
    .cmd_func = nullptr,
    .cmd_argt = 278L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tlast] = {
    .cmd_name = "tlast",
    .cmd_func = nullptr,
    .cmd_argt = 258L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tlmenu] = {
    .cmd_name = "tlmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_tlnoremenu] = {
    .cmd_name = "tlnoremenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_tlunmenu] = {
    .cmd_name = "tlunmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_tmenu] = {
    .cmd_name = "tmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_tmap] = {
    .cmd_name = "tmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tmapclear] = {
    .cmd_name = "tmapclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tnext] = {
    .cmd_name = "tnext",
    .cmd_func = nullptr,
    .cmd_argt = 4355L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_tnoremap] = {
    .cmd_name = "tnoremap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_topleft] = {
    .cmd_name = "topleft",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tprevious] = {
    .cmd_name = "tprevious",
    .cmd_func = nullptr,
    .cmd_argt = 4355L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_trewind] = {
    .cmd_name = "trewind",
    .cmd_func = nullptr,
    .cmd_argt = 4355L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_trust] = {
    .cmd_name = "trust",
    .cmd_func = nullptr,
    .cmd_argt = 16777500L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_try] = {
    .cmd_name = "try",
    .cmd_func = nullptr,
    .cmd_argt = 17563904L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tselect] = {
    .cmd_name = "tselect",
    .cmd_func = nullptr,
    .cmd_argt = 278L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tunmenu] = {
    .cmd_name = "tunmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_tunmap] = {
    .cmd_name = "tunmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_undo] = {
    .cmd_name = "undo",
    .cmd_func = nullptr,
    .cmd_argt = 17306883L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_undojoin] = {
    .cmd_name = "undojoin",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_undolist] = {
    .cmd_name = "undolist",
    .cmd_func = nullptr,
    .cmd_argt = 17301760L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_unabbreviate] = {
    .cmd_name = "unabbreviate",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_unhide] = {
    .cmd_name = "unhide",
    .cmd_func = nullptr,
    .cmd_argt = 1281L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_unlet] = {
    .cmd_name = "unlet",
    .cmd_func = nullptr,
    .cmd_argt = 17563782L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_unlockvar] = {
    .cmd_name = "unlockvar",
    .cmd_func = nullptr,
    .cmd_argt = 17563782L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_unmap] = {
    .cmd_name = "unmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312006L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_unmenu] = {
    .cmd_name = "unmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17312006L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_unsilent] = {
    .cmd_name = "unsilent",
    .cmd_func = nullptr,
    .cmd_argt = 17565828L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_update] = {
    .cmd_name = "update",
    .cmd_func = nullptr,
    .cmd_argt = 131455L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_vglobal] = {
    .cmd_name = "vglobal",
    .cmd_func = nullptr,
    .cmd_argt = 17301605L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_version] = {
    .cmd_name = "version",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_verbose] = {
    .cmd_name = "verbose",
    .cmd_func = nullptr,
    .cmd_argt = 17565829L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_vertical] = {
    .cmd_name = "vertical",
    .cmd_func = nullptr,
    .cmd_argt = 2180L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_visual] = {
    .cmd_name = "visual",
    .cmd_func = nullptr,
    .cmd_argt = 147742L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_view] = {
    .cmd_name = "view",
    .cmd_func = nullptr,
    .cmd_argt = 147742L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_vimgrep] = {
    .cmd_name = "vimgrep",
    .cmd_func = nullptr,
    .cmd_argt = 16779663L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_vimgrepadd] = {
    .cmd_name = "vimgrepadd",
    .cmd_func = nullptr,
    .cmd_argt = 16779663L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_viusage] = {
    .cmd_name = "viusage",
    .cmd_func = nullptr,
    .cmd_argt = 256L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_vmap] = {
    .cmd_name = "vmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_vmapclear] = {
    .cmd_name = "vmapclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_vmenu] = {
    .cmd_name = "vmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_vnoremap] = {
    .cmd_name = "vnoremap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_vnew] = {
    .cmd_name = "vnew",
    .cmd_func = nullptr,
    .cmd_argt = 147743L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_vnoremenu] = {
    .cmd_name = "vnoremenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_vsplit] = {
    .cmd_name = "vsplit",
    .cmd_func = nullptr,
    .cmd_argt = 147743L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_vunmap] = {
    .cmd_name = "vunmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_vunmenu] = {
    .cmd_name = "vunmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_write] = {
    .cmd_name = "write",
    .cmd_func = nullptr,
    .cmd_argt = 17432959L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_wNext] = {
    .cmd_name = "wNext",
    .cmd_func = nullptr,
    .cmd_argt = 131423L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_wall] = {
    .cmd_name = "wall",
    .cmd_func = nullptr,
    .cmd_argt = 17301762L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_while] = {
    .cmd_name = "while",
    .cmd_func = nullptr,
    .cmd_argt = 17565700L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_winsize] = {
    .cmd_name = "winsize",
    .cmd_func = nullptr,
    .cmd_argt = 388L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_wincmd] = {
    .cmd_name = "wincmd",
    .cmd_func = nullptr,
    .cmd_argt = 17302677L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_windo] = {
    .cmd_name = "windo",
    .cmd_argt = 2213L,
    .cmd_addr_type = ADDR_WINDOWS
  },
  [CMD_winpos] = {
    .cmd_name = "winpos",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_wnext] = {
    .cmd_name = "wnext",
    .cmd_func = nullptr,
    .cmd_argt = 131359L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_wprevious] = {
    .cmd_name = "wprevious",
    .cmd_func = nullptr,
    .cmd_argt = 131359L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_wq] = {
    .cmd_name = "wq",
    .cmd_func = nullptr,
    .cmd_argt = 131455L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_wqall] = {
    .cmd_name = "wqall",
    .cmd_func = nullptr,
    .cmd_argt = 131358L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_wshada] = {
    .cmd_name = "wshada",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_wundo] = {
    .cmd_name = "wundo",
    .cmd_func = nullptr,
    .cmd_argt = 158L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_wviminfo] = {
    .cmd_name = "wviminfo",
    .cmd_func = nullptr,
    .cmd_argt = 17301790L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_xit] = {
    .cmd_name = "xit",
    .cmd_func = nullptr,
    .cmd_argt = 17432959L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_xall] = {
    .cmd_name = "xall",
    .cmd_func = nullptr,
    .cmd_argt = 258L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_xmap] = {
    .cmd_name = "xmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_xmapclear] = {
    .cmd_name = "xmapclear",
    .cmd_func = nullptr,
    .cmd_argt = 17301764L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_xmenu] = {
    .cmd_name = "xmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_xnoremap] = {
    .cmd_name = "xnoremap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_xnoremenu] = {
    .cmd_name = "xnoremenu",
    .cmd_func = nullptr,
    .cmd_argt = 17316101L,
    .cmd_addr_type = ADDR_OTHER
  },
  [CMD_xunmap] = {
    .cmd_name = "xunmap",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_xunmenu] = {
    .cmd_name = "xunmenu",
    .cmd_func = nullptr,
    .cmd_argt = 17312004L,
    .cmd_addr_type = ADDR_NONE
  },
  [CMD_yank] = {
    .cmd_name = "yank",
    .cmd_func = nullptr,
    .cmd_argt = 17303361L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_z] = {
    .cmd_name = "z",
    .cmd_func = nullptr,
    .cmd_argt = 19398983L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_bang] = {
    .cmd_name = "!",
    .cmd_func = nullptr,
    .cmd_argt = 17301583L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_pound] = {
    .cmd_name = "#",
    .cmd_func = nullptr,
    .cmd_argt = 19400001L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_and] = {
    .cmd_name = "&",
    .cmd_func = nullptr,
    .cmd_argt = 18350149L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_lshift] = {
    .cmd_name = "<",
    .cmd_func = nullptr,
    .cmd_argt = 20448577L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_equal] = {
    .cmd_name = "=",
    .cmd_func = nullptr,
    .cmd_argt = 17432613L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_rshift] = {
    .cmd_name = ">",
    .cmd_func = nullptr,
    .cmd_argt = 20448577L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_at] = {
    .cmd_name = "@",
    .cmd_func = nullptr,
    .cmd_argt = 17301829L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_tilde] = {
    .cmd_name = "~",
    .cmd_func = nullptr,
    .cmd_argt = 18350149L,
    .cmd_addr_type = ADDR_LINES
  },
  [CMD_Next] = {
    .cmd_name = "Next",
    .cmd_func = nullptr,
    .cmd_argt = 148743L,
    .cmd_addr_type = ADDR_OTHER
  },
};

const uint16_t cmdidxs1[26] = {
  /*  a  */ 0,
  /*  b  */ 20,
  /*  c  */ 44,
  /*  d  */ 109,
  /*  e  */ 131,
  /*  f  */ 152,
  /*  g  */ 167,
  /*  h  */ 173,
  /*  i  */ 181,
  /*  j  */ 199,
  /*  k  */ 201,
  /*  l  */ 206,
  /*  m  */ 267,
  /*  n  */ 285,
  /*  o  */ 302,
  /*  p  */ 313,
  /*  q  */ 351,
  /*  r  */ 354,
  /*  s  */ 375,
  /*  t  */ 440,
  /*  u  */ 485,
  /*  v  */ 496,
  /*  w  */ 514,
  /*  x  */ 529,
  /*  y  */ 538,
  /*  z  */ 539,
};

const uint8_t cmdidxs2[26][26] = {
  /*           a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p   q   r   s   t   u   v   w   x   y   z */
  /*  a  */ {  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  5,  6,  0,  0,  0,  7, 16,  0, 17,  0,  0,  0,  0,  0, },
  /*  b  */ {  2,  0,  0,  5,  6,  8,  0,  0,  0,  0,  0,  9, 10, 11, 12, 13,  0, 14,  0,  0,  0,  0, 23,  0,  0,  0, },
  /*  c  */ {  3, 12, 16, 18, 20, 22, 25,  0,  0,  0,  0, 34, 38, 41, 47, 57, 59, 60,  0,  0, 61,  0, 64,  0,  0,  0, },
  /*  d  */ {  0,  0,  0,  0,  0,  0,  0,  0,  6, 15,  0, 16,  0,  0, 17,  0,  0, 19, 20,  0,  0,  0,  0,  0,  0,  0, },
  /*  e  */ {  1,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  7,  9, 10,  0,  0,  0,  0,  0,  0,  0, 16,  0, 17,  0,  0, },
  /*  f  */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0, },
  /*  g  */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  2,  0,  0,  4,  5,  0,  0,  0,  0, },
  /*  h  */ {  0,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, },
  /*  i  */ {  1,  0,  0,  0,  0,  3,  0,  0,  0,  4,  0,  5,  6,  0,  0,  0,  0,  0, 13,  0, 15,  0,  0,  0,  0,  0, },
  /*  j  */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0, },
  /*  k  */ {  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, },
  /*  l  */ {  3, 11, 15, 18, 19, 23, 26, 31,  0,  0,  0, 33, 36, 39, 43, 49,  0, 51, 60, 52, 53, 57, 59,  0,  0,  0, },
  /*  m  */ {  1,  0,  0,  0,  7,  0,  0,  0,  0,  0, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 16, },
  /*  n  */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  5,  7,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0, },
  /*  o  */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  4,  0,  7,  0,  0,  0,  0,  8,  0, 10,  0,  0,  0, },
  /*  p  */ {  1,  0,  3,  0,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, 10,  0,  0, 15, 16, 25,  0, 26,  0, 27,  0, },
  /*  q  */ {  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, },
  /*  r  */ {  0,  0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0, 15, 20,  0,  0,  0,  0, },
  /*  s  */ {  2,  6, 15,  0, 17, 21,  0,  0, 23,  0,  0, 26, 28, 32, 36, 38,  0, 47,  0, 48,  0, 60, 61,  0, 62,  0, },
  /*  t  */ {  4,  0,  1,  0, 24, 25,  0, 26,  0, 27,  0, 28, 32, 35, 37, 38,  0, 39, 42,  0, 43,  0,  0,  0,  0,  0, },
  /*  u  */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, },
  /*  v  */ {  0,  0,  0,  0,  1,  0,  0,  0,  4,  0,  0,  0,  9, 12,  0,  0,  0,  0, 15,  0, 16,  0,  0,  0,  0,  0, },
  /*  w  */ {  2,  0,  0,  0,  0,  0,  0,  3,  4,  0,  0,  0,  0,  8,  0,  9, 10,  0, 12,  0, 13, 14,  0,  0,  0,  0, },
  /*  x  */ {  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  5,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0, },
  /*  y  */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, },
  /*  z  */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, },
};
