#include "EvalFuncDefs.hpp"

#include <cstring>

EvalFuncDef functions[] = {
  { "id", 1, 1, BASE_NONE, false},
  { "or", 2, 2, 1, false},
  { "tr", 3, 3, 1, false},
  { "abs", 1, 1, 1, false},
  { "add", 2, 2, 1, false},
  { "and", 2, 2, 1, false},
  { "col", 1, 2, 1, false},
  { "cos", 1, 1, 1, false},
  { "exp", 1, 1, 1, false},
  { "get", 2, 3, 1, false},
  { "has", 1, 1, BASE_NONE, false},
  { "len", 1, 1, 1, false},
  { "log", 1, 1, 1, false},
  { "map", 2, 2, 1, false},
  { "max", 1, 1, 1, false},
  { "min", 1, 1, 1, false},
  { "pow", 2, 2, 1, false},
  { "sin", 1, 1, 1, false},
  { "tan", 1, 1, 1, false},
  { "xor", 2, 2, 1, false},
  { "hlID", 1, 1, 1, false},
  { "glob", 1, 4, 1, false},
  { "argc", 0, 1, BASE_NONE, false},
  { "fmod", 2, 2, 1, false},
  { "rand", 0, 1, 1, false},
  { "line", 1, 2, 1, false},
  { "mode", 0, 1, 1, false},
  { "type", 1, 1, 1, true},
  { "cosh", 1, 1, 1, false},
  { "sinh", 1, 1, 1, false},
  { "tanh", 1, 1, 1, false},
  { "call", 2, 3, 1, false},
  { "ceil", 1, 1, 1, false},
  { "eval", 1, 1, 1, false},
  { "trim", 1, 3, 1, false},
  { "asin", 1, 1, 1, false},
  { "atan", 1, 1, 1, false},
  { "join", 1, 2, 1, false},
  { "uniq", 1, 3, 1, false},
  { "acos", 1, 1, 1, false},
  { "keys", 1, 1, 1, false},
  { "sort", 1, 3, 1, false},
  { "sqrt", 1, 1, 1, false},
  { "wait", 2, 3, BASE_NONE, false},
  { "argv", 0, 2, BASE_NONE, false},
  { "copy", 1, 1, 1, false},
  { "match", 2, 4, 1, false},
  { "range", 1, 3, 1, false},
  { "iconv", 3, 3, 1, true},
  { "chdir", 1, 1, 1, false},
  { "winnr", 0, 1, 1, false},
  { "mkdir", 1, 3, 1, false},
  { "floor", 1, 1, 1, false},
  { "empty", 1, 1, 1, false},
  { "index", 2, 4, 1, false},
  { "input", 1, 3, 1, false},
  { "count", 2, 4, 1, false},
  { "log10", 1, 1, 1, false},
  { "round", 1, 1, 1, false},
  { "split", 1, 3, 1, false},
  { "srand", 0, 1, 1, false},
  { "trunc", 1, 1, 1, false},
  { "isinf", 1, 1, 1, false},
  { "isnan", 1, 1, 1, false},
  { "atan2", 2, 2, 1, false},
  { "items", 1, 1, 1, false},
  { "bufnr", 0, 2, 1, false},
  { "synID", 3, 3, BASE_NONE, false},
  { "sha256", 1, 1, 1, false},
  { "append", 2, 2, 2, false},
  { "bufadd", 1, 1, 1, false},
  { "expand", 1, 3, 1, false},
  { "extend", 2, 3, 1, false},
  { "getcwd", 0, 2, 1, false},
  { "getpid", 0, 0, BASE_NONE, true},
  { "jobpid", 1, 1, BASE_NONE, false},
  { "browse", 4, 4, BASE_NONE, false},
  { "delete", 1, 2, 1, false},
  { "escape", 2, 2, 1, true},
  { "reduce", 2, 3, 1, false},
  { "remove", 2, 3, 1, false},
  { "rename", 2, 2, 1, false},
  { "printf", 1, MAX_FUNC_ARGS, 2, false},
  { "getreg", 0, 3, 1, false},
  { "maparg", 1, 4, 1, false},
  { "setreg", 2, 3, 2, false},
  { "string", 1, 1, 1, false},
  { "search", 1, 5, 1, false},
  { "pyeval", 1, 1, 1, false},
  { "wincol", 0, 0, BASE_NONE, false},
  { "system", 1, 2, 1, false},
  { "strlen", 1, 1, 1, false},
  { "ctxpop", 0, 0, BASE_NONE, false},
  { "cursor", 1, 3, 1, false},
  { "filter", 2, 2, 1, false},
  { "histnr", 1, 1, 1, false},
  { "str2nr", 1, 3, 1, false},
  { "exists", 1, 1, 1, false},
  { "getpos", 1, 1, 1, false},
  { "setpos", 2, 2, 2, false},
  { "values", 1, 1, 1, false},
  { "ctxget", 0, 1, BASE_NONE, false},
  { "ctxset", 1, 2, BASE_NONE, false},
  { "indent", 1, 1, 1, false},
  { "insert", 2, 3, 1, false},
  { "invert", 1, 1, 1, false},
  { "mapset", 3, 3, 1, false},
  { "repeat", 2, 2, 1, true},
  { "getenv", 1, 1, 1, false},
  { "setenv", 2, 2, 2, false},
  { "argidx", 0, 0, BASE_NONE, false},
  { "stridx", 2, 3, 1, true},
  { "nr2char", 1, 2, 1, false},
  { "py3eval", 1, 1, 1, false},
  { "char2nr", 1, 2, 1, true},
  { "charcol", 1, 2, 1, false},
  { "charidx", 2, 3, 1, false},
  { "flatten", 1, 2, 1, false},
  { "luaeval", 1, 2, 1, false},
  { "readdir", 1, 2, 1, false},
  { "jobsend", 2, 2, BASE_NONE, false},
  { "jobstop", 1, 1, BASE_NONE, false},
  { "jobwait", 1, 2, BASE_NONE, false},
  { "libcall", 3, 3, 3, false},
  { "rpcstop", 1, 1, BASE_NONE, false},
  { "indexof", 2, 3, 1, false},
  { "stdpath", 1, 1, BASE_NONE, true},
  { "execute", 1, 2, 1, false},
  { "exepath", 1, 1, 1, false},
  { "bufload", 1, 1, 1, false},
  { "bufname", 0, 1, 1, false},
  { "taglist", 1, 2, 1, false},
  { "reltime", 0, 2, 1, true},
  { "tolower", 1, 1, 1, true},
  { "cindent", 1, 1, 1, false},
  { "confirm", 1, 4, 1, false},
  { "finddir", 1, 3, 1, false},
  { "funcref", 1, 3, 1, false},
  { "winline", 0, 0, BASE_NONE, false},
  { "strpart", 2, 4, 1, true},
  { "strridx", 2, 3, 1, false},
  { "virtcol", 1, 1, 1, false},
  { "has_key", 2, 2, 1, false},
  { "histadd", 2, 2, 2, false},
  { "histdel", 1, 2, 1, false},
  { "histget", 1, 2, 1, false},
  { "resolve", 1, 1, 1, true},
  { "byteidx", 2, 2, 1, true},
  { "getchar", 0, 1, BASE_NONE, false},
  { "getline", 1, 2, 1, false},
  { "gettext", 1, 1, 1, false},
  { "setline", 2, 2, 2, false},
  { "toupper", 1, 1, 1, true},
  { "environ", 0, 0, BASE_NONE, true},
  { "reverse", 1, 1, 1, false},
  { "ctxpush", 0, 1, BASE_NONE, false},
  { "ctxsize", 0, 0, BASE_NONE, false},
  { "pyxeval", 1, 1, 1, false},
  { "str2list", 1, 2, 1, false},
  { "api_info", 0, 0, BASE_NONE, true},
  { "float2nr", 1, 1, 1, false},
  { "globpath", 2, 5, 2, false},
  { "winbufnr", 1, 1, 1, false},
  { "function", 1, 3, 1, false},
  { "jobclose", 1, 2, BASE_NONE, false},
  { "mapcheck", 1, 3, 1, false},
  { "matchadd", 2, 5, 1, false},
  { "matcharg", 1, 1, 1, false},
  { "matchend", 2, 4, 1, false},
  { "matchstr", 2, 4, 1, false},
  { "strchars", 1, 2, 1, false},
  { "feedkeys", 1, 2, 1, false},
  { "findfile", 1, 3, 1, false},
  { "foldtext", 0, 0, BASE_NONE, false},
  { "readblob", 1, 3, 1, false},
  { "readfile", 1, 3, 1, false},
  { "getfperm", 1, 1, 1, true},
  { "getfsize", 1, 1, 1, true},
  { "getftime", 1, 1, 1, true},
  { "getftype", 1, 1, 1, true},
  { "setfperm", 2, 2, 1, false},
  { "strftime", 1, 2, 1, false},
  { "tagfiles", 0, 0, BASE_NONE, false},
  { "perleval", 1, 1, 1, false},
  { "hasmapto", 1, 3, 1, false},
  { "nvim__id", 1, 1, BASE_NONE, false},
  { "nvim_cmd", 2, 2, BASE_NONE, false},
  { "nvim_put", 4, 4, BASE_NONE, false},
  { "submatch", 1, 2, 1, false},
  { "termopen", 1, 2, BASE_NONE, false},
  { "changenr", 0, 0, BASE_NONE, false},
  { "chansend", 2, 2, BASE_NONE, false},
  { "islocked", 1, 1, 1, false},
  { "undofile", 1, 1, 1, false},
  { "undotree", 0, 0, BASE_NONE, false},
  { "complete", 2, 2, 2, false},
  { "deepcopy", 1, 2, 1, false},
  { "simplify", 1, 1, 1, false},
  { "strptime", 2, 2, 1, false},
  { "swapinfo", 1, 1, 1, false},
  { "swapname", 1, 1, 1, false},
  { "tempname", 0, 0, BASE_NONE, false},
  { "jobstart", 1, 2, BASE_NONE, false},
  { "rpcstart", 1, 2, BASE_NONE, false},
  { "synstack", 2, 2, BASE_NONE, false},
  { "hostname", 0, 0, BASE_NONE, true},
  { "keytrans", 1, 1, 1, false},
  { "list2str", 1, 2, 1, false},
  { "strtrans", 1, 1, 1, true},
  { "menu_get", 1, 2, BASE_NONE, false},
  { "bufwinid", 1, 1, 1, false},
  { "bufwinnr", 1, 1, 1, false},
  { "strwidth", 1, 1, 1, true},
  { "winwidth", 1, 1, 1, false},
  { "hlexists", 1, 1, 1, false},
  { "rubyeval", 1, 1, 1, false},
  { "blob2list", 1, 1, 1, false},
  { "byte2line", 1, 1, 1, false},
  { "line2byte", 1, 1, 1, false},
  { "list2blob", 1, 1, 1, false},
  { "synIDattr", 2, 3, 1, false},
  { "diff_hlID", 2, 2, 1, false},
  { "menu_info", 1, 2, 1, false},
  { "nvim_echo", 3, 3, BASE_NONE, false},
  { "nvim_eval", 1, 1, BASE_NONE, false},
  { "nvim_exec", 2, 2, BASE_NONE, false},
  { "sign_jump", 3, 3, 1, false},
  { "gettabvar", 2, 3, 1, false},
  { "libcallnr", 3, 3, 3, false},
  { "settabvar", 3, 3, 3, false},
  { "tabpagenr", 0, 1, BASE_NONE, false},
  { "winlayout", 0, 1, 1, false},
  { "chanclose", 1, 2, BASE_NONE, false},
  { "charclass", 1, 1, 1, false},
  { "searchpos", 1, 5, 1, false},
  { "wordcount", 0, 0, BASE_NONE, false},
  { "soundfold", 1, 1, 1, false},
  { "jobresize", 3, 3, BASE_NONE, false},
  { "screencol", 0, 0, BASE_NONE, false},
  { "screenpos", 3, 3, 1, false},
  { "screenrow", 0, 0, BASE_NONE, false},
  { "winheight", 1, 1, 1, false},
  { "writefile", 2, 3, 1, false},
  { "getqflist", 0, 1, BASE_NONE, false},
  { "setqflist", 1, 3, 1, false},
  { "str2float", 1, 1, 1, false},
  { "win_getid", 0, 2, 1, false},
  { "matchlist", 2, 4, 1, false},
  { "arglistid", 0, 2, BASE_NONE, false},
  { "buflisted", 1, 1, 1, false},
  { "getwinpos", 0, 1, 1, false},
  { "getwinvar", 2, 3, 1, false},
  { "setwinvar", 3, 3, 3, false},
  { "foldlevel", 1, 1, 1, false},
  { "localtime", 0, 0, BASE_NONE, false},
  { "getcmdpos", 0, 0, BASE_NONE, false},
  { "setcmdpos", 1, 1, 1, false},
  { "expandcmd", 1, 2, 1, false},
  { "extendnew", 2, 3, 1, false},
  { "bufloaded", 1, 1, 1, false},
  { "rpcnotify", 2, MAX_FUNC_ARGS, BASE_NONE, false},
  { "stdioopen", 1, 1, BASE_NONE, false},
  { "interrupt", 0, 0, BASE_NONE, false},
  { "browsedir", 2, 2, BASE_NONE, false},
  { "inputlist", 1, 1, 1, false},
  { "inputsave", 0, 0, BASE_NONE, false},
  { "getbufvar", 2, 3, 1, false},
  { "getcurpos", 0, 1, 1, false},
  { "setbufvar", 3, 3, 3, false},
  { "bufexists", 1, 1, 1, false},
  { "timer_info", 0, 1, 1, false},
  { "timer_stop", 1, 1, 1, false},
  { "getcharmod", 0, 0, BASE_NONE, false},
  { "getcharpos", 1, 1, 1, false},
  { "getcharstr", 0, 1, BASE_NONE, false},
  { "setcharpos", 2, 2, 2, false},
  { "strcharlen", 1, 1, 1, false},
  { "debugbreak", 1, 1, 1, false},
  { "gettabinfo", 0, 1, 1, false},
  { "getloclist", 1, 2, BASE_NONE, false},
  { "setloclist", 2, 4, 2, false},
  { "getcmdline", 0, 0, BASE_NONE, false},
  { "getcmdtype", 0, 0, BASE_NONE, false},
  { "setcmdline", 1, 2, 1, false},
  { "win_id2win", 1, 1, 1, false},
  { "flattennew", 1, 2, 1, false},
  { "nvim_exec2", 2, 2, BASE_NONE, false},
  { "pum_getpos", 0, 0, BASE_NONE, false},
  { "getbufinfo", 0, 1, 1, false},
  { "getbufline", 2, 3, 1, false},
  { "matchfuzzy", 2, 3, 1, false},
  { "setbufline", 3, 3, 3, false},
  { "getreginfo", 0, 1, 1, false},
  { "getregtype", 0, 1, 1, false},
  { "searchdecl", 1, 3, 1, false},
  { "searchpair", 3, 7, BASE_NONE, false},
  { "nvim_input", 1, 1, BASE_NONE, false},
  { "substitute", 4, 4, 1, false},
  { "foldclosed", 1, 1, 1, false},
  { "visualmode", 0, 1, BASE_NONE, false},
  { "reltimestr", 1, 1, 1, true},
  { "systemlist", 1, 3, 1, false},
  { "getwininfo", 0, 1, 1, false},
  { "getwinposx", 0, 0, BASE_NONE, false},
  { "getwinposy", 0, 0, BASE_NONE, false},
  { "lispindent", 1, 1, 1, false},
  { "screenattr", 2, 2, 1, false},
  { "screenchar", 2, 2, 1, false},
  { "win_gotoid", 1, 1, 1, false},
  { "nvim_paste", 3, 3, BASE_NONE, false},
  { "sign_place", 4, 5, 1, false},
  { "rpcrequest", 2, MAX_FUNC_ARGS, BASE_NONE, false},
  { "foreground", 0, 0, BASE_NONE, false},
  { "serverlist", 0, 0, BASE_NONE, false},
  { "serverstop", 1, 1, BASE_NONE, false},
  { "pumvisible", 0, 0, BASE_NONE, false},
  { "winrestcmd", 0, 0, BASE_NONE, false},
  { "executable", 1, 1, 1, true},
  { "getmatches", 0, 1, BASE_NONE, false},
  { "setmatches", 1, 2, 1, false},
  { "strgetchar", 2, 2, 1, false},
  { "synIDtrans", 1, 1, 1, false},
  { "shiftwidth", 0, 1, 1, false},
  { "nvim__stats", 0, 0, BASE_NONE, false},
  { "timer_pause", 2, 2, 1, false},
  { "timer_start", 2, 3, 1, false},
  { "matchaddpos", 2, 5, 1, false},
  { "strcharpart", 2, 3, 1, true},
  { "haslocaldir", 0, 2, 1, false},
  { "msgpackdump", 1, 2, BASE_NONE, false},
  { "byteidxcomp", 2, 2, 1, true},
  { "inputdialog", 1, 3, 1, false},
  { "json_decode", 1, 1, 1, false},
  { "matchdelete", 1, 2, 1, false},
  { "sign_define", 1, 2, 1, false},
  { "fnameescape", 1, 1, 1, true},
  { "isdirectory", 1, 1, 1, true},
  { "json_encode", 1, 1, 1, false},
  { "shellescape", 1, 2, 1, false},
  { "win_gettype", 0, 1, 1, false},
  { "diff_filler", 1, 1, 1, false},
  { "gettagstack", 0, 1, 1, false},
  { "nvim_get_hl", 2, 2, BASE_NONE, false},
  { "settagstack", 2, 3, 2, false},
  { "pathshorten", 1, 2, 1, false},
  { "searchcount", 0, 1, 1, false},
  { "highlightID", 1, 1, 1, false},
  { "win_findbuf", 1, 1, 1, false},
  { "fnamemodify", 2, 2, 1, true},
  { "getjumplist", 0, 2, 1, false},
  { "getfontname", 0, 1, BASE_NONE, false},
  { "nvim_notify", 3, 3, BASE_NONE, false},
  { "screenchars", 2, 2, 1, false},
  { "fullcommand", 1, 1, 1, false},
  { "sockconnect", 2, 3, BASE_NONE, false},
  { "virtcol2col", 3, 3, 1, false},
  { "digraph_get", 1, 1, 1, false},
  { "digraph_set", 2, 2, 1, false},
  { "buffer_name", 0, 1, 1, false},
  { "getmarklist", 0, 1, 1, false},
  { "glob2regpat", 1, 1, 1, false},
  { "serverstart", 0, 1, BASE_NONE, false},
  { "inputsecret", 1, 2, 1, false},
  { "matchstrpos", 2, 4, 1, false},
  { "nvim_set_hl", 3, 3, BASE_NONE, false},
  { "winrestview", 1, 1, 1, false},
  { "assert_true", 1, 2, 1, false},
  { "getmousepos", 0, 0, BASE_NONE, false},
  { "winsaveview", 0, 0, BASE_NONE, false},
  { "win_execute", 2, 3, 2, false},
  { "tabpagewinnr", 1, 2, 1, false},
  { "did_filetype", 0, 0, BASE_NONE, true},
  { "clearmatches", 0, 1, 1, false},
  { "eventhandler", 0, 0, BASE_NONE, false},
  { "prevnonblank", 1, 1, 1, false},
  { "spellbadword", 0, 1, 1, false},
  { "spellsuggest", 1, 3, 1, false},
  { "msgpackparse", 1, 1, BASE_NONE, false},
  { "reg_recorded", 0, 0, BASE_NONE, false},
  { "sign_unplace", 1, 2, 1, false},
  { "nvim__unpack", 1, 1, BASE_NONE, false},
  { "nvim_command", 1, 1, BASE_NONE, false},
  { "nvim_del_var", 1, 1, BASE_NONE, false},
  { "nvim_get_var", 1, 1, BASE_NONE, false},
  { "nvim_set_var", 2, 2, BASE_NONE, false},
  { "filereadable", 1, 1, 1, true},
  { "filewritable", 1, 1, 1, true},
  { "reltimefloat", 1, 1, 1, true},
  { "wildmenumode", 0, 0, BASE_NONE, false},
  { "complete_add", 1, 1, 1, false},
  { "synconcealed", 2, 2, BASE_NONE, false},
  { "inputrestore", 0, 0, BASE_NONE, false},
  { "screenstring", 2, 2, 1, false},
  { "assert_beeps", 1, 1, 1, false},
  { "assert_equal", 2, 3, 2, false},
  { "assert_fails", 1, 5, 1, false},
  { "assert_false", 1, 2, 1, false},
  { "assert_match", 2, 3, 2, false},
  { "gettabwinvar", 3, 4, 1, false},
  { "settabwinvar", 4, 4, 4, false},
  { "nextnonblank", 1, 1, 1, false},
  { "timer_stopall", 0, 0, BASE_NONE, false},
  { "getchangelist", 0, 1, 1, false},
  { "getcharsearch", 0, 0, BASE_NONE, false},
  { "setcharsearch", 1, 1, 1, false},
  { "win_screenpos", 1, 1, 1, false},
  { "appendbufline", 3, 3, 3, false},
  { "getcmdwintype", 0, 0, BASE_NONE, false},
  { "nvim_del_mark", 1, 1, BASE_NONE, false},
  { "win_id2tabwin", 1, 1, 1, false},
  { "complete_info", 0, 1, 1, false},
  { "deletebufline", 2, 3, 1, false},
  { "reg_recording", 0, 0, BASE_NONE, false},
  { "getbufoneline", 2, 2, 1, false},
  { "matchfuzzypos", 2, 3, 1, false},
  { "nvim_feedkeys", 3, 3, BASE_NONE, false},
  { "nvim_get_mark", 2, 2, BASE_NONE, false},
  { "nvim_get_mode", 0, 0, BASE_NONE, false},
  { "nvim_get_proc", 1, 1, BASE_NONE, false},
  { "nvim_get_vvar", 1, 1, BASE_NONE, false},
  { "searchpairpos", 3, 7, BASE_NONE, false},
  { "foldclosedend", 1, 1, 1, false},
  { "getcellwidths", 0, 0, BASE_NONE, false},
  { "nvim_list_uis", 0, 0, BASE_NONE, false},
  { "setcellwidths", 1, 1, 1, false},
  { "getcompletion", 2, 3, 1, false},
  { "nvim_open_win", 3, 3, BASE_NONE, false},
  { "win_splitmove", 2, 3, 1, false},
  { "buffer_exists", 1, 1, 1, false},
  { "buffer_number", 0, 1, 1, false},
  { "file_readable", 1, 1, 1, false},
  { "getscriptinfo", 0, 0, BASE_NONE, false},
  { "nvim_set_vvar", 2, 2, BASE_NONE, false},
  { "nvim_strwidth", 1, 1, BASE_NONE, false},
  { "assert_nobeep", 1, 1, 1, false},
  { "assert_report", 1, 1, 1, false},
  { "sign_undefine", 0, 1, 1, false},
  { "nvim_win_hide", 1, 1, BASE_NONE, false},
  { "reg_executing", 0, 0, BASE_NONE, false},
  { "nvim__id_array", 1, 1, BASE_NONE, false},
  { "nvim__id_float", 1, 1, BASE_NONE, false},
  { "dictwatcheradd", 3, 3, BASE_NONE, false},
  { "dictwatcherdel", 3, 3, BASE_NONE, false},
  { "last_buffer_nr", 0, 0, BASE_NONE, false},
  { "complete_check", 0, 0, BASE_NONE, false},
  { "foldtextresult", 1, 1, 1, false},
  { "nvim_err_write", 1, 1, BASE_NONE, false},
  { "garbagecollect", 0, 1, BASE_NONE, false},
  { "sign_getplaced", 0, 2, 1, false},
  { "tabpagebuflist", 0, 1, 1, false},
  { "nvim_list_bufs", 0, 0, BASE_NONE, false},
  { "nvim_list_wins", 0, 0, BASE_NONE, false},
  { "nvim_open_term", 2, 2, BASE_NONE, false},
  { "nvim_out_write", 1, 1, BASE_NONE, false},
  { "nvim_parse_cmd", 2, 2, BASE_NONE, false},
  { "sign_placelist", 1, 1, 1, false},
  { "nvim_set_hl_ns", 1, 1, BASE_NONE, false},
  { "assert_inrange", 3, 4, 3, false},
  { "nvim_win_close", 2, 2, BASE_NONE, false},
  { "windowsversion", 0, 0, BASE_NONE, true},
  { "nvim__buf_stats", 1, 1, BASE_NONE, false},
  { "nvim_buf_attach", 3, 3, BASE_NONE, false},
  { "nvim_buf_delete", 2, 2, BASE_NONE, false},
  { "nvim_create_buf", 2, 2, BASE_NONE, false},
  { "getcmdcompltype", 0, 0, BASE_NONE, false},
  { "getcmdscreenpos", 0, 0, BASE_NONE, false},
  { "nvim_del_keymap", 2, 2, BASE_NONE, false},
  { "nvim_get_keymap", 1, 1, BASE_NONE, false},
  { "nvim_get_option", 1, 1, BASE_NONE, false},
  { "sign_getdefined", 0, 1, 1, false},
  { "nvim_list_chans", 0, 0, BASE_NONE, false},
  { "digraph_getlist", 0, 1, 1, false},
  { "digraph_setlist", 1, 1, 1, false},
  { "nvim_set_keymap", 4, 4, BASE_NONE, false},
  { "nvim_set_option", 2, 2, BASE_NONE, false},
  { "strdisplaywidth", 1, 2, 1, false},
  { "assert_notequal", 2, 3, 2, false},
  { "assert_notmatch", 2, 3, 2, false},
  { "highlight_exists", 1, 1, 1, false},
  { "nvim_del_autocmd", 1, 1, BASE_NONE, false},
  { "sign_unplacelist", 1, 1, 1, false},
  { "assert_exception", 1, 2, BASE_NONE, false},
  { "getcursorcharpos", 0, 1, 1, false},
  { "nvim_get_context", 1, 1, BASE_NONE, false},
  { "setcursorcharpos", 1, 3, 1, false},
  { "nvim_buf_del_var", 2, 2, BASE_NONE, false},
  { "nvim_win_del_var", 2, 2, BASE_NONE, false},
  { "nvim__screenshot", 1, 1, BASE_NONE, false},
  { "nvim_buf_get_var", 2, 2, BASE_NONE, false},
  { "nvim_win_get_buf", 1, 1, BASE_NONE, false},
  { "nvim_win_get_var", 2, 2, BASE_NONE, false},
  { "nvim_buf_set_var", 3, 3, BASE_NONE, false},
  { "nvim_win_set_buf", 2, 2, BASE_NONE, false},
  { "nvim_win_set_var", 3, 3, BASE_NONE, false},
  { "nvim_input_mouse", 6, 6, BASE_NONE, false},
  { "prompt_getprompt", 1, 1, 1, false},
  { "prompt_setprompt", 2, 2, 1, false},
  { "assert_equalfile", 2, 3, 1, false},
  { "nvim_err_writeln", 1, 1, BASE_NONE, false},
  { "nvim__get_lib_dir", 0, 0, BASE_NONE, false},
  { "nvim__get_runtime", 3, 3, BASE_NONE, false},
  { "nvim_load_context", 1, 1, BASE_NONE, false},
  { "nvim_get_autocmds", 1, 1, BASE_NONE, false},
  { "nvim_get_commands", 1, 1, BASE_NONE, false},
  { "nvim_buf_del_mark", 2, 2, BASE_NONE, false},
  { "nvim_buf_get_mark", 2, 2, BASE_NONE, false},
  { "nvim_buf_get_name", 1, 1, BASE_NONE, false},
  { "nvim_buf_get_text", 6, 6, BASE_NONE, false},
  { "nvim_get_hl_by_id", 2, 2, BASE_NONE, false},
  { "nvim_buf_is_valid", 1, 1, BASE_NONE, false},
  { "nvim_win_is_valid", 1, 1, BASE_NONE, false},
  { "nvim_buf_set_mark", 5, 5, BASE_NONE, false},
  { "nvim_buf_set_name", 2, 2, BASE_NONE, false},
  { "nvim_buf_set_text", 6, 6, BASE_NONE, false},
  { "nvim__inspect_cell", 3, 3, BASE_NONE, false},
  { "nvim_buf_get_lines", 4, 4, BASE_NONE, false},
  { "nvim_buf_is_loaded", 1, 1, BASE_NONE, false},
  { "nvim_buf_set_lines", 5, 5, BASE_NONE, false},
  { "nvim_call_function", 2, 2, BASE_NONE, false},
  { "nvim_exec_autocmds", 2, 2, BASE_NONE, false},
  { "nvim_get_chan_info", 1, 1, BASE_NONE, false},
  { "nvim_get_color_map", 0, 0, BASE_NONE, false},
  { "nvim_list_tabpages", 0, 0, BASE_NONE, false},
  { "win_move_separator", 2, 2, 1, false},
  { "prompt_setcallback", 2, 2, 1, false},
  { "nvim_win_get_width", 1, 1, BASE_NONE, false},
  { "nvim_win_set_hl_ns", 2, 2, BASE_NONE, false},
  { "nvim_win_set_width", 2, 2, BASE_NONE, false},
  { "nvim_get_hl_by_name", 2, 2, BASE_NONE, false},
  { "nvim_set_hl_ns_fast", 1, 1, BASE_NONE, false},
  { "nvim_buf_line_count", 1, 1, BASE_NONE, false},
  { "nvim_buf_del_keymap", 3, 3, BASE_NONE, false},
  { "nvim_buf_get_keymap", 2, 2, BASE_NONE, false},
  { "nvim_buf_set_keymap", 5, 5, BASE_NONE, false},
  { "nvim_win_get_height", 1, 1, BASE_NONE, false},
  { "nvim_win_set_height", 2, 2, BASE_NONE, false},
  { "nvim_buf_get_offset", 2, 2, BASE_NONE, false},
  { "nvim_create_augroup", 2, 2, BASE_NONE, false},
  { "nvim__id_dictionary", 1, 1, BASE_NONE, false},
  { "nvim_clear_autocmds", 1, 1, BASE_NONE, false},
  { "nvim_win_get_config", 1, 1, BASE_NONE, false},
  { "nvim_win_set_config", 2, 2, BASE_NONE, false},
  { "nvim_buf_get_option", 2, 2, BASE_NONE, false},
  { "nvim_buf_set_option", 3, 3, BASE_NONE, false},
  { "nvim_get_namespaces", 0, 0, BASE_NONE, false},
  { "nvim_win_get_option", 2, 2, BASE_NONE, false},
  { "nvim_win_set_option", 3, 3, BASE_NONE, false},
  { "prompt_setinterrupt", 2, 2, 1, false},
  { "win_move_statusline", 2, 2, 1, false},
  { "nvim_create_autocmd", 2, 2, BASE_NONE, false},
  { "test_write_list_log", 1, 1, BASE_NONE, false},
  { "nvim_buf_get_number", 1, 1, BASE_NONE, false},
  { "nvim_command_output", 1, 1, BASE_NONE, false},
  { "nvim_win_get_cursor", 1, 1, BASE_NONE, false},
  { "nvim_win_get_number", 1, 1, BASE_NONE, false},
  { "nvim_win_set_cursor", 2, 2, BASE_NONE, false},
  { "nvim_buf_del_extmark", 3, 3, BASE_NONE, false},
  { "nvim_buf_set_extmark", 5, 5, BASE_NONE, false},
  { "nvim_win_get_tabpage", 1, 1, BASE_NONE, false},
  { "nvim_get_current_buf", 0, 0, BASE_NONE, false},
  { "nvim_set_current_buf", 1, 1, BASE_NONE, false},
  { "nvim_set_current_dir", 1, 1, BASE_NONE, false},
  { "nvim_eval_statusline", 2, 2, BASE_NONE, false},
  { "nvim_get_option_info", 1, 1, BASE_NONE, false},
  { "nvim_tabpage_del_var", 2, 2, BASE_NONE, false},
  { "nvim_tabpage_get_var", 2, 2, BASE_NONE, false},
  { "nvim_tabpage_set_var", 3, 3, BASE_NONE, false},
  { "nvim_get_current_win", 0, 0, BASE_NONE, false},
  { "nvim_set_current_win", 1, 1, BASE_NONE, false},
  { "nvim_tabpage_get_win", 1, 1, BASE_NONE, false},
  { "nvim_tabpage_is_valid", 1, 1, BASE_NONE, false},
  { "nvim_del_current_line", 0, 0, BASE_NONE, false},
  { "nvim_get_current_line", 0, 0, BASE_NONE, false},
  { "nvim_set_current_line", 1, 1, BASE_NONE, false},
  { "nvim_parse_expression", 3, 3, BASE_NONE, false},
  { "nvim_buf_get_commands", 2, 2, BASE_NONE, false},
  { "nvim_buf_get_extmarks", 5, 5, BASE_NONE, false},
  { "nvim_win_get_position", 1, 1, BASE_NONE, false},
  { "nvim_get_option_info2", 2, 2, BASE_NONE, false},
  { "nvim_get_option_value", 2, 2, BASE_NONE, false},
  { "nvim_set_option_value", 3, 3, BASE_NONE, false},
  { "nvim_get_runtime_file", 2, 2, BASE_NONE, false},
  { "nvim__runtime_inspect", 0, 0, BASE_NONE, false},
  { "nvim_create_namespace", 1, 1, BASE_NONE, false},
  { "nvim_del_user_command", 1, 1, BASE_NONE, false},
  { "nvim_replace_termcodes", 4, 4, BASE_NONE, false},
  { "nvim_buf_add_highlight", 6, 6, BASE_NONE, false},
  { "nvim_tabpage_list_wins", 1, 1, BASE_NONE, false},
  { "nvim_get_hl_id_by_name", 1, 1, BASE_NONE, false},
  { "nvim_get_color_by_name", 1, 1, BASE_NONE, false},
  { "nvim__buf_redraw_range", 3, 3, BASE_NONE, false},
  { "nvim_get_proc_children", 1, 1, BASE_NONE, false},
  { "nvim_del_augroup_by_id", 1, 1, BASE_NONE, false},
  { "nvim_call_dict_function", 3, 3, BASE_NONE, false},
  { "test_garbagecollect_now", 0, 0, BASE_NONE, false},
  { "nvim_list_runtime_paths", 0, 0, BASE_NONE, false},
  { "nvim_tabpage_get_number", 1, 1, BASE_NONE, false},
  { "nvim_create_user_command", 3, 3, BASE_NONE, false},
  { "nvim_buf_clear_highlight", 4, 4, BASE_NONE, false},
  { "nvim_buf_clear_namespace", 4, 4, BASE_NONE, false},
  { "nvim_del_augroup_by_name", 1, 1, BASE_NONE, false},
  { "nvim_get_current_tabpage", 0, 0, BASE_NONE, false},
  { "nvim_set_current_tabpage", 1, 1, BASE_NONE, false},
  { "nvim_buf_get_changedtick", 1, 1, BASE_NONE, false},
  { "nvim_get_all_options_info", 0, 0, BASE_NONE, false},
  { "nvim_buf_del_user_command", 2, 2, BASE_NONE, false},
  { "nvim_buf_set_virtual_text", 5, 5, BASE_NONE, false},
  { "nvim_buf_get_extmark_by_id", 4, 4, BASE_NONE, false},
  { "nvim_select_popupmenu_item", 4, 4, BASE_NONE, false},
  { "nvim_buf_create_user_command", 4, 4, BASE_NONE, false},
};

int find_internal_func_hash(const char *str, int len) {
    int low = 0, high = 0;
    switch (len) {
        case 2: switch (str[0]) {
            case 'i': low = 0; high = 1; break;
            case 'o': low = 1; high = 2; break;
            case 't': low = 2; high = 3; break;
            default: break;
        }
        break;

        case 3: switch (str[0]) {
            case 'a': low = 3; high = 6; break;
            case 'c': low = 6; high = 8; break;
            case 'e': low = 8; high = 9; break;
            case 'g': low = 9; high = 10; break;
            case 'h': low = 10; high = 11; break;
            case 'l': low = 11; high = 13; break;
            case 'm': low = 13; high = 16; break;
            case 'p': low = 16; high = 17; break;
            case 's': low = 17; high = 18; break;
            case 't': low = 18; high = 19; break;
            case 'x': low = 19; high = 20; break;
            default: break;
        }
        break;

        case 4: switch (str[3]) {
            case 'D': low = 20; high = 21; break;
            case 'b': low = 21; high = 22; break;
            case 'c': low = 22; high = 23; break;
            case 'd': low = 23; high = 25; break;
            case 'e': low = 25; high = 28; break;
            case 'h': low = 28; high = 31; break;
            case 'l': low = 31; high = 34; break;
            case 'm': low = 34; high = 35; break;
            case 'n': low = 35; high = 38; break;
            case 'q': low = 38; high = 39; break;
            case 's': low = 39; high = 41; break;
            case 't': low = 41; high = 44; break;
            case 'v': low = 44; high = 45; break;
            case 'y': low = 45; high = 46; break;
            default: break;
        }
        break;

        case 5: switch (str[1]) {
            case 'a': low = 46; high = 48; break;
            case 'c': low = 48; high = 49; break;
            case 'h': low = 49; high = 50; break;
            case 'i': low = 50; high = 51; break;
            case 'k': low = 51; high = 52; break;
            case 'l': low = 52; high = 53; break;
            case 'm': low = 53; high = 54; break;
            case 'n': low = 54; high = 56; break;
            case 'o': low = 56; high = 59; break;
            case 'p': low = 59; high = 60; break;
            case 'r': low = 60; high = 62; break;
            case 's': low = 62; high = 64; break;
            case 't': low = 64; high = 66; break;
            case 'u': low = 66; high = 67; break;
            case 'y': low = 67; high = 68; break;
            default: break;
        }
        break;

        case 6: switch (str[5]) {
            case '6': low = 68; high = 69; break;
            case 'd': low = 69; high = 76; break;
            case 'e': low = 76; high = 82; break;
            case 'f': low = 82; high = 83; break;
            case 'g': low = 83; high = 87; break;
            case 'h': low = 87; high = 88; break;
            case 'l': low = 88; high = 90; break;
            case 'm': low = 90; high = 91; break;
            case 'n': low = 91; high = 92; break;
            case 'p': low = 92; high = 93; break;
            case 'r': low = 93; high = 97; break;
            case 's': low = 97; high = 101; break;
            case 't': low = 101; high = 108; break;
            case 'v': low = 108; high = 110; break;
            case 'x': low = 110; high = 112; break;
            default: break;
        }
        break;

        case 7: switch (str[2]) {
            case '2': low = 112; high = 113; break;
            case '3': low = 113; high = 114; break;
            case 'a': low = 114; high = 120; break;
            case 'b': low = 120; high = 124; break;
            case 'c': low = 124; high = 125; break;
            case 'd': low = 125; high = 127; break;
            case 'e': low = 127; high = 129; break;
            case 'f': low = 129; high = 131; break;
            case 'g': low = 131; high = 132; break;
            case 'l': low = 132; high = 134; break;
            case 'n': low = 134; high = 139; break;
            case 'r': low = 139; high = 142; break;
            case 's': low = 142; high = 147; break;
            case 't': low = 147; high = 152; break;
            case 'u': low = 152; high = 153; break;
            case 'v': low = 153; high = 155; break;
            case 'x': low = 155; high = 158; break;
            default: break;
        }
        break;

        case 8: switch (str[3]) {
            case '2': low = 158; high = 159; break;
            case '_': low = 159; high = 160; break;
            case 'a': low = 160; high = 161; break;
            case 'b': low = 161; high = 163; break;
            case 'c': low = 163; high = 171; break;
            case 'd': low = 171; high = 176; break;
            case 'f': low = 176; high = 183; break;
            case 'l': low = 183; high = 184; break;
            case 'm': low = 184; high = 190; break;
            case 'n': low = 190; high = 192; break;
            case 'o': low = 192; high = 195; break;
            case 'p': low = 195; high = 202; break;
            case 's': low = 202; high = 205; break;
            case 't': low = 205; high = 209; break;
            case 'u': low = 209; high = 210; break;
            case 'w': low = 210; high = 214; break;
            case 'x': low = 214; high = 215; break;
            case 'y': low = 215; high = 216; break;
            default: break;
        }
        break;

        case 9: switch (str[4]) {
            case '2': low = 216; high = 220; break;
            case 'D': low = 220; high = 221; break;
            case '_': low = 221; high = 227; break;
            case 'a': low = 227; high = 232; break;
            case 'c': low = 232; high = 236; break;
            case 'd': low = 236; high = 237; break;
            case 'e': low = 237; high = 243; break;
            case 'f': low = 243; high = 246; break;
            case 'g': low = 246; high = 247; break;
            case 'h': low = 247; high = 248; break;
            case 'i': low = 248; high = 253; break;
            case 'l': low = 253; high = 255; break;
            case 'm': low = 255; high = 257; break;
            case 'n': low = 257; high = 259; break;
            case 'o': low = 259; high = 262; break;
            case 'r': low = 262; high = 263; break;
            case 's': low = 263; high = 264; break;
            case 't': low = 264; high = 266; break;
            case 'u': low = 266; high = 269; break;
            case 'x': low = 269; high = 270; break;
            default: break;
        }
        break;

        case 10: switch (str[5]) {
            case '_': low = 270; high = 272; break;
            case 'a': low = 272; high = 277; break;
            case 'b': low = 277; high = 279; break;
            case 'c': low = 279; high = 281; break;
            case 'd': low = 281; high = 285; break;
            case 'e': low = 285; high = 288; break;
            case 'f': low = 288; high = 292; break;
            case 'g': low = 292; high = 294; break;
            case 'h': low = 294; high = 296; break;
            case 'i': low = 296; high = 298; break;
            case 'l': low = 298; high = 300; break;
            case 'm': low = 300; high = 302; break;
            case 'n': low = 302; high = 308; break;
            case 'o': low = 308; high = 309; break;
            case 'p': low = 309; high = 311; break;
            case 'q': low = 311; high = 312; break;
            case 'r': low = 312; high = 315; break;
            case 's': low = 315; high = 317; break;
            case 't': low = 317; high = 322; break;
            case 'w': low = 322; high = 323; break;
            default: break;
        }
        break;

        case 11: switch (str[5]) {
            case '_': low = 323; high = 326; break;
            case 'a': low = 326; high = 328; break;
            case 'c': low = 328; high = 330; break;
            case 'd': low = 330; high = 335; break;
            case 'e': low = 335; high = 340; break;
            case 'f': low = 340; high = 341; break;
            case 'g': low = 341; high = 344; break;
            case 'h': low = 344; high = 346; break;
            case 'i': low = 346; high = 348; break;
            case 'm': low = 348; high = 350; break;
            case 'n': low = 350; high = 353; break;
            case 'o': low = 353; high = 356; break;
            case 'p': low = 356; high = 358; break;
            case 'r': low = 358; high = 362; break;
            case 's': low = 362; high = 366; break;
            case 't': low = 366; high = 367; break;
            case 'u': low = 367; high = 368; break;
            case 'v': low = 368; high = 369; break;
            case 'x': low = 369; high = 370; break;
            default: break;
        }
        break;

        case 12: switch (str[2]) {
            case 'b': low = 370; high = 371; break;
            case 'd': low = 371; high = 372; break;
            case 'e': low = 372; high = 377; break;
            case 'g': low = 377; high = 380; break;
            case 'i': low = 380; high = 385; break;
            case 'l': low = 385; high = 389; break;
            case 'm': low = 389; high = 390; break;
            case 'n': low = 390; high = 391; break;
            case 'p': low = 391; high = 392; break;
            case 'r': low = 392; high = 393; break;
            case 's': low = 393; high = 398; break;
            case 't': low = 398; high = 400; break;
            case 'x': low = 400; high = 401; break;
            default: break;
        }
        break;

        case 13: switch (str[5]) {
            case '_': low = 401; high = 402; break;
            case 'a': low = 402; high = 405; break;
            case 'c': low = 405; high = 406; break;
            case 'd': low = 406; high = 410; break;
            case 'e': low = 410; high = 413; break;
            case 'f': low = 413; high = 416; break;
            case 'g': low = 416; high = 420; break;
            case 'h': low = 420; high = 421; break;
            case 'l': low = 421; high = 425; break;
            case 'm': low = 425; high = 426; break;
            case 'o': low = 426; high = 427; break;
            case 'p': low = 427; high = 428; break;
            case 'r': low = 428; high = 432; break;
            case 's': low = 432; high = 434; break;
            case 't': low = 434; high = 436; break;
            case 'u': low = 436; high = 437; break;
            case 'w': low = 437; high = 438; break;
            case 'x': low = 438; high = 439; break;
            default: break;
        }
        break;

        case 14: switch (str[5]) {
            case '_': low = 439; high = 441; break;
            case 'a': low = 441; high = 443; break;
            case 'b': low = 443; high = 444; break;
            case 'e': low = 444; high = 447; break;
            case 'g': low = 447; high = 450; break;
            case 'l': low = 450; high = 452; break;
            case 'o': low = 452; high = 454; break;
            case 'p': low = 454; high = 456; break;
            case 's': low = 456; high = 457; break;
            case 't': low = 457; high = 458; break;
            case 'w': low = 458; high = 460; break;
            default: break;
        }
        break;

        case 15: switch (str[5]) {
            case '_': low = 460; high = 461; break;
            case 'b': low = 461; high = 463; break;
            case 'c': low = 463; high = 464; break;
            case 'd': low = 464; high = 467; break;
            case 'g': low = 467; high = 470; break;
            case 'l': low = 470; high = 471; break;
            case 'p': low = 471; high = 473; break;
            case 's': low = 473; high = 476; break;
            case 't': low = 476; high = 478; break;
            default: break;
        }
        break;

        case 16: switch (str[9]) {
            case '_': low = 478; high = 479; break;
            case 'a': low = 479; high = 481; break;
            case 'c': low = 481; high = 485; break;
            case 'd': low = 485; high = 487; break;
            case 'e': low = 487; high = 488; break;
            case 'g': low = 488; high = 491; break;
            case 's': low = 491; high = 494; break;
            case 't': low = 494; high = 497; break;
            case 'u': low = 497; high = 498; break;
            case 'w': low = 498; high = 499; break;
            default: break;
        }
        break;

        case 17: switch (str[9]) {
            case '_': low = 499; high = 502; break;
            case 'a': low = 502; high = 503; break;
            case 'c': low = 503; high = 504; break;
            case 'd': low = 504; high = 505; break;
            case 'g': low = 505; high = 508; break;
            case 'h': low = 508; high = 509; break;
            case 'i': low = 509; high = 511; break;
            case 's': low = 511; high = 514; break;
            default: break;
        }
        break;

        case 18: switch (str[5]) {
            case '_': low = 514; high = 515; break;
            case 'b': low = 515; high = 518; break;
            case 'c': low = 518; high = 519; break;
            case 'e': low = 519; high = 520; break;
            case 'g': low = 520; high = 522; break;
            case 'l': low = 522; high = 523; break;
            case 'o': low = 523; high = 524; break;
            case 't': low = 524; high = 525; break;
            case 'w': low = 525; high = 528; break;
            default: break;
        }
        break;

        case 19: switch (str[14]) {
            case '_': low = 528; high = 530; break;
            case 'c': low = 530; high = 531; break;
            case 'e': low = 531; high = 536; break;
            case 'f': low = 536; high = 537; break;
            case 'g': low = 537; high = 538; break;
            case 'o': low = 538; high = 542; break;
            case 'p': low = 542; high = 547; break;
            case 'r': low = 547; high = 548; break;
            case 's': low = 548; high = 549; break;
            case 't': low = 549; high = 551; break;
            case 'u': low = 551; high = 556; break;
            default: break;
        }
        break;

        case 20: switch (str[17]) {
            case 'a': low = 556; high = 559; break;
            case 'b': low = 559; high = 561; break;
            case 'd': low = 561; high = 562; break;
            case 'i': low = 562; high = 563; break;
            case 'n': low = 563; high = 564; break;
            case 'v': low = 564; high = 567; break;
            case 'w': low = 567; high = 570; break;
            default: break;
        }
        break;

        case 21: switch (str[9]) {
            case 'a': low = 570; high = 571; break;
            case 'c': low = 571; high = 574; break;
            case 'e': low = 574; high = 575; break;
            case 'g': low = 575; high = 578; break;
            case 'o': low = 578; high = 581; break;
            case 'r': low = 581; high = 582; break;
            case 't': low = 582; high = 584; break;
            case 'u': low = 584; high = 585; break;
            default: break;
        }
        break;

        case 22: switch (str[10]) {
            case 'c': low = 585; high = 586; break;
            case 'd': low = 586; high = 587; break;
            case 'g': low = 587; high = 588; break;
            case 'l': low = 588; high = 589; break;
            case 'o': low = 589; high = 590; break;
            case 'r': low = 590; high = 592; break;
            case 'u': low = 592; high = 593; break;
            default: break;
        }
        break;
        
        case 23: switch (str[5]) {
            case 'c': low = 593; high = 594; break;
            case 'g': low = 594; high = 595; break;
            case 'l': low = 595; high = 596; break;
            case 't': low = 596; high = 597; break;
            default: break;
        }
        break;

        case 24: switch (str[11]) {
            case '_': low = 597; high = 598; break;
            case 'e': low = 598; high = 600; break;
            case 'g': low = 600; high = 601; break;
            case 'r': low = 601; high = 603; break;
            case 't': low = 603; high = 604; break;
            default: break;
        }
        break;

        case 25: switch (str[9]) {
            case 'a': low = 604; high = 605; break;
            case 'd': low = 605; high = 606; break;
            case 's': low = 606; high = 607; break;
            default: break;
        }
        break;

        case 26: switch (str[5]) {
            case 'b': low = 607; high = 608; break;
            case 's': low = 608; high = 609; break;
            default: break;
        }
        break;

        case 28: low = 609; high = 610; break;

        default: break;
    }
    for (int i = low; i < high; i++) {
        if (!strncmp(str, functions[i].name, len)) {
            return i;
        }
    }
    return -1;
}
