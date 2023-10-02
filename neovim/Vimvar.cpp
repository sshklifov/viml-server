#include "Vimvar.hpp"
#include "NeovimBase.hpp"

#include <StaticStringMap.hpp>

// values for vv_flags:
#define VV_COMPAT      1               // compatible, also used without "v:"
#define VV_RO          2               // read-only
#define VV_RO_SBX      4               // read-only in the sandbox

StaticStringMap<vimvar> creator();

vimvar get_vimvar(const char* arg, int len) {
    static const StaticStringMap<vimvar> vimvars = creator();

    if (len < 2) {
        return {VAR_UNKNOWN, 0};
    } else if (*arg != 'v' && *arg != ':') {
        if (strncmp(arg, "version", len) == 0) {
            return vimvars["version"];
        } else {
            return {VAR_UNKNOWN, 0};
        }
    } else {
        int i = vimvars.find(arg, len);
        if (i < 0) {
            return {VAR_UNKNOWN, 0};
        } else {
            return vimvars[i];
        }
    }
}

StaticStringMap<vimvar> creator() {
    StaticStringMap<vimvar> vimvars;
    vimvars.emplace("_null_blob", VAR_BLOB, VV_RO);
    vimvars.emplace("_null_dict", VAR_DICT, VV_RO);
    vimvars.emplace("_null_list", VAR_LIST, VV_RO);
    vimvars.emplace("_null_string", VAR_STRING, VV_RO);
    vimvars.emplace("argv", VAR_LIST, VV_RO);
    vimvars.emplace("beval_bufnr", VAR_NUMBER, VV_RO);
    vimvars.emplace("beval_col", VAR_NUMBER, VV_RO);
    vimvars.emplace("beval_lnum", VAR_NUMBER, VV_RO);
    vimvars.emplace("beval_text", VAR_STRING, VV_RO);
    vimvars.emplace("beval_winid", VAR_NUMBER, VV_RO);
    vimvars.emplace("beval_winnr", VAR_NUMBER, VV_RO);
    vimvars.emplace("char", VAR_STRING, 0);
    vimvars.emplace("charconvert_from", VAR_STRING, VV_RO);
    vimvars.emplace("charconvert_to", VAR_STRING, VV_RO);
    vimvars.emplace("cmdarg", VAR_STRING, VV_RO);
    vimvars.emplace("cmdbang", VAR_NUMBER, VV_RO);
    vimvars.emplace("collate", VAR_STRING, VV_RO);
    vimvars.emplace("completed_item", VAR_DICT, 0);
    vimvars.emplace("count", VAR_NUMBER, VV_RO);
    vimvars.emplace("count1", VAR_NUMBER, VV_RO);
    vimvars.emplace("ctype", VAR_STRING, VV_RO);
    vimvars.emplace("dying", VAR_NUMBER, VV_RO);
    vimvars.emplace("echospace", VAR_NUMBER, VV_RO);
    vimvars.emplace("errmsg", VAR_STRING, 0);
    vimvars.emplace("errors", VAR_LIST, 0);
    vimvars.emplace("event", VAR_DICT, VV_RO);
    vimvars.emplace("exception", VAR_STRING, VV_RO);
    vimvars.emplace("exiting", VAR_NUMBER, VV_RO);
    vimvars.emplace("false", VAR_BOOL, VV_RO);
    vimvars.emplace("fcs_choice", VAR_STRING, 0);
    vimvars.emplace("fcs_reason", VAR_STRING, VV_RO);
    vimvars.emplace("fname", VAR_STRING, VV_RO);
    vimvars.emplace("fname_diff", VAR_STRING, VV_RO);
    vimvars.emplace("fname_in", VAR_STRING, VV_RO);
    vimvars.emplace("fname_new", VAR_STRING, VV_RO);
    vimvars.emplace("fname_out", VAR_STRING, VV_RO);
    vimvars.emplace("folddashes", VAR_STRING, VV_RO_SBX);
    vimvars.emplace("foldend", VAR_NUMBER, VV_RO_SBX);
    vimvars.emplace("foldlevel", VAR_NUMBER, VV_RO_SBX);
    vimvars.emplace("foldstart", VAR_NUMBER, VV_RO_SBX);
    vimvars.emplace("hlsearch", VAR_NUMBER, 0);
    vimvars.emplace("insertmode", VAR_STRING, VV_RO);
    vimvars.emplace("key", VAR_UNKNOWN, VV_RO);
    vimvars.emplace("lang", VAR_STRING, VV_RO);
    vimvars.emplace("lc_time", VAR_STRING, VV_RO);
    vimvars.emplace("lnum", VAR_NUMBER, VV_RO_SBX);
    vimvars.emplace("lua", VAR_PARTIAL, VV_RO);
    vimvars.emplace("maxcol", VAR_NUMBER, VV_RO);
    vimvars.emplace("mouse_col", VAR_NUMBER, 0);
    vimvars.emplace("mouse_lnum", VAR_NUMBER, 0);
    vimvars.emplace("mouse_win", VAR_NUMBER, 0);
    vimvars.emplace("mouse_winid", VAR_NUMBER, 0);
    vimvars.emplace("msgpack_types", VAR_DICT, VV_RO);
    vimvars.emplace("null", VAR_SPECIAL, VV_RO);
    vimvars.emplace("numbermax", VAR_NUMBER, VV_RO);
    vimvars.emplace("numbermin", VAR_NUMBER, VV_RO);
    vimvars.emplace("numbersize", VAR_NUMBER, VV_RO);
    vimvars.emplace("oldfiles", VAR_LIST, 0);
    vimvars.emplace("operator", VAR_STRING, VV_RO);
    vimvars.emplace("option_command", VAR_STRING, VV_RO);
    vimvars.emplace("option_new", VAR_STRING, VV_RO);
    vimvars.emplace("option_old", VAR_STRING, VV_RO);
    vimvars.emplace("option_oldglobal", VAR_STRING, VV_RO);
    vimvars.emplace("option_oldlocal", VAR_STRING, VV_RO);
    vimvars.emplace("option_type", VAR_STRING, VV_RO);
    vimvars.emplace("prevcount", VAR_NUMBER, VV_RO);
    vimvars.emplace("profiling", VAR_NUMBER, VV_RO);
    vimvars.emplace("progname", VAR_STRING, VV_RO);
    vimvars.emplace("progpath", VAR_STRING, VV_RO);
    vimvars.emplace("register", VAR_STRING, VV_RO);
    vimvars.emplace("relnum", VAR_NUMBER, VV_RO);
    vimvars.emplace("scrollstart", VAR_STRING, 0);
    vimvars.emplace("searchforward", VAR_NUMBER, 0);
    vimvars.emplace("servername", VAR_STRING, VV_RO);
    vimvars.emplace("shell_error", VAR_NUMBER, VV_RO);
    vimvars.emplace("statusmsg", VAR_STRING, 0);
    vimvars.emplace("stderr", VAR_NUMBER, VV_RO);
    vimvars.emplace("swapchoice", VAR_STRING, 0);
    vimvars.emplace("swapcommand", VAR_STRING, VV_RO);
    vimvars.emplace("swapname", VAR_STRING, VV_RO);
    vimvars.emplace("t_blob", VAR_NUMBER, VV_RO);
    vimvars.emplace("t_bool", VAR_NUMBER, VV_RO);
    vimvars.emplace("t_dict", VAR_NUMBER, VV_RO);
    vimvars.emplace("t_float", VAR_NUMBER, VV_RO);
    vimvars.emplace("t_func", VAR_NUMBER, VV_RO);
    vimvars.emplace("t_list", VAR_NUMBER, VV_RO);
    vimvars.emplace("t_number", VAR_NUMBER, VV_RO);
    vimvars.emplace("t_string", VAR_NUMBER, VV_RO);
    vimvars.emplace("termresponse", VAR_STRING, VV_RO);
    vimvars.emplace("testing", VAR_NUMBER, 0);
    vimvars.emplace("this_session", VAR_STRING, 0);
    vimvars.emplace("throwpoint", VAR_STRING, VV_RO);
    vimvars.emplace("true", VAR_BOOL, VV_RO);
    vimvars.emplace("val", VAR_UNKNOWN, VV_RO);
    vimvars.emplace("version", VAR_NUMBER, VV_COMPAT + VV_RO);
    vimvars.emplace("vim_did_enter", VAR_NUMBER, VV_RO);
    vimvars.emplace("virtnum", VAR_NUMBER, VV_RO);
    vimvars.emplace("warningmsg", VAR_STRING, 0);
    vimvars.emplace("windowid", VAR_NUMBER, VV_RO_SBX);

    return vimvars;
}
