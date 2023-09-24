#include "Options.hpp"
#include "Eval.hpp"
#include <cstring>

int OptionsMap::findVarType(const char* name, int n) const {
    if (name[0] < 'a' || name[0] > 'z') {
        return VAR_UNKNOWN;
    }

    int quickIdx = name[0] - 'a';
    for (int i = quickTab[quickIdx]; i < opts.count(); ++i) {
        const char* key = opts.map[i].key;
        if (key[0] != name[0]) {
            break;
        }
        if (strncmp(key, name, n) == 0) {
            return opts[i];
        }
    }
    return VAR_UNKNOWN;
}

const OptionsMap& OptionsMap::getSingleton() {
    static OptionsMap s;
    return s;
}

OptionsMap::OptionsMap() {
    opts.emplace("aleph", VAR_NUMBER);
    opts.emplace("al", VAR_NUMBER);
    opts.emplace("arabic", VAR_BOOL);
    opts.emplace("arab", VAR_BOOL);
    opts.emplace("arabicshape", VAR_BOOL);
    opts.emplace("arshape", VAR_BOOL);
    opts.emplace("allowrevins", VAR_BOOL);
    opts.emplace("ari", VAR_BOOL);
    opts.emplace("ambiwidth", VAR_STRING);
    opts.emplace("ambw", VAR_STRING);
    opts.emplace("autochdir", VAR_BOOL);
    opts.emplace("acd", VAR_BOOL);
    opts.emplace("autoindent", VAR_BOOL);
    opts.emplace("ai", VAR_BOOL);
    opts.emplace("autoread", VAR_BOOL);
    opts.emplace("ar", VAR_BOOL);
    opts.emplace("autowrite", VAR_BOOL);
    opts.emplace("aw", VAR_BOOL);
    opts.emplace("autowriteall", VAR_BOOL);
    opts.emplace("awa", VAR_BOOL);
    opts.emplace("background", VAR_STRING);
    opts.emplace("bg", VAR_STRING);
    opts.emplace("backspace", VAR_STRING);
    opts.emplace("bs", VAR_STRING);
    opts.emplace("backup", VAR_BOOL);
    opts.emplace("bk", VAR_BOOL);
    opts.emplace("backupcopy", VAR_STRING);
    opts.emplace("bkc", VAR_STRING);
    opts.emplace("backupdir", VAR_STRING);
    opts.emplace("bdir", VAR_STRING);
    opts.emplace("backupext", VAR_STRING);
    opts.emplace("bex", VAR_STRING);
    opts.emplace("backupskip", VAR_STRING);
    opts.emplace("bsk", VAR_STRING);
    opts.emplace("belloff", VAR_STRING);
    opts.emplace("bo", VAR_STRING);
    opts.emplace("binary", VAR_BOOL);
    opts.emplace("bin", VAR_BOOL);
    opts.emplace("bomb", VAR_BOOL);
    opts.emplace("breakat", VAR_STRING);
    opts.emplace("brk", VAR_STRING);
    opts.emplace("breakindent", VAR_BOOL);
    opts.emplace("bri", VAR_BOOL);
    opts.emplace("breakindentopt", VAR_STRING);
    opts.emplace("briopt", VAR_STRING);
    opts.emplace("browsedir", VAR_STRING);
    opts.emplace("bsdir", VAR_STRING);
    opts.emplace("bufhidden", VAR_STRING);
    opts.emplace("bh", VAR_STRING);
    opts.emplace("buflisted", VAR_BOOL);
    opts.emplace("bl", VAR_BOOL);
    opts.emplace("buftype", VAR_STRING);
    opts.emplace("bt", VAR_STRING);
    opts.emplace("casemap", VAR_STRING);
    opts.emplace("cmp", VAR_STRING);
    opts.emplace("cdhome", VAR_BOOL);
    opts.emplace("cdh", VAR_BOOL);
    opts.emplace("cdpath", VAR_STRING);
    opts.emplace("cd", VAR_STRING);
    opts.emplace("cedit", VAR_STRING);
    opts.emplace("channel", VAR_NUMBER);
    opts.emplace("charconvert", VAR_STRING);
    opts.emplace("ccv", VAR_STRING);
    opts.emplace("cindent", VAR_BOOL);
    opts.emplace("cin", VAR_BOOL);
    opts.emplace("cinkeys", VAR_STRING);
    opts.emplace("cink", VAR_STRING);
    opts.emplace("cinoptions", VAR_STRING);
    opts.emplace("cino", VAR_STRING);
    opts.emplace("cinwords", VAR_STRING);
    opts.emplace("cinw", VAR_STRING);
    opts.emplace("cinscopedecls", VAR_STRING);
    opts.emplace("cinsd", VAR_STRING);
    opts.emplace("clipboard", VAR_STRING);
    opts.emplace("cb", VAR_STRING);
    opts.emplace("cmdheight", VAR_NUMBER);
    opts.emplace("ch", VAR_NUMBER);
    opts.emplace("cmdwinheight", VAR_NUMBER);
    opts.emplace("cwh", VAR_NUMBER);
    opts.emplace("colorcolumn", VAR_STRING);
    opts.emplace("cc", VAR_STRING);
    opts.emplace("columns", VAR_NUMBER);
    opts.emplace("co", VAR_NUMBER);
    opts.emplace("comments", VAR_STRING);
    opts.emplace("com", VAR_STRING);
    opts.emplace("commentstring", VAR_STRING);
    opts.emplace("cms", VAR_STRING);
    opts.emplace("compatible", VAR_BOOL);
    opts.emplace("cp", VAR_BOOL);
    opts.emplace("complete", VAR_STRING);
    opts.emplace("cpt", VAR_STRING);
    opts.emplace("concealcursor", VAR_STRING);
    opts.emplace("cocu", VAR_STRING);
    opts.emplace("conceallevel", VAR_NUMBER);
    opts.emplace("cole", VAR_NUMBER);
    opts.emplace("completefunc", VAR_STRING);
    opts.emplace("cfu", VAR_STRING);
    opts.emplace("completeopt", VAR_STRING);
    opts.emplace("cot", VAR_STRING);
    opts.emplace("completeslash", VAR_STRING);
    opts.emplace("csl", VAR_STRING);
    opts.emplace("confirm", VAR_BOOL);
    opts.emplace("cf", VAR_BOOL);
    opts.emplace("copyindent", VAR_BOOL);
    opts.emplace("ci", VAR_BOOL);
    opts.emplace("cpoptions", VAR_STRING);
    opts.emplace("cpo", VAR_STRING);
    opts.emplace("cursorbind", VAR_BOOL);
    opts.emplace("crb", VAR_BOOL);
    opts.emplace("cursorcolumn", VAR_BOOL);
    opts.emplace("cuc", VAR_BOOL);
    opts.emplace("cursorline", VAR_BOOL);
    opts.emplace("cul", VAR_BOOL);
    opts.emplace("cursorlineopt", VAR_STRING);
    opts.emplace("culopt", VAR_STRING);
    opts.emplace("debug", VAR_STRING);
    opts.emplace("define", VAR_STRING);
    opts.emplace("def", VAR_STRING);
    opts.emplace("delcombine", VAR_BOOL);
    opts.emplace("deco", VAR_BOOL);
    opts.emplace("dictionary", VAR_STRING);
    opts.emplace("dict", VAR_STRING);
    opts.emplace("diff", VAR_BOOL);
    opts.emplace("diffexpr", VAR_STRING);
    opts.emplace("dex", VAR_STRING);
    opts.emplace("diffopt", VAR_STRING);
    opts.emplace("dip", VAR_STRING);
    opts.emplace("digraph", VAR_BOOL);
    opts.emplace("dg", VAR_BOOL);
    opts.emplace("directory", VAR_STRING);
    opts.emplace("dir", VAR_STRING);
    opts.emplace("display", VAR_STRING);
    opts.emplace("dy", VAR_STRING);
    opts.emplace("eadirection", VAR_STRING);
    opts.emplace("ead", VAR_STRING);
    opts.emplace("edcompatible", VAR_BOOL);
    opts.emplace("ed", VAR_BOOL);
    opts.emplace("emoji", VAR_BOOL);
    opts.emplace("emo", VAR_BOOL);
    opts.emplace("encoding", VAR_STRING);
    opts.emplace("enc", VAR_STRING);
    opts.emplace("endoffile", VAR_BOOL);
    opts.emplace("eof", VAR_BOOL);
    opts.emplace("endofline", VAR_BOOL);
    opts.emplace("eol", VAR_BOOL);
    opts.emplace("equalalways", VAR_BOOL);
    opts.emplace("ea", VAR_BOOL);
    opts.emplace("equalprg", VAR_STRING);
    opts.emplace("ep", VAR_STRING);
    opts.emplace("errorbells", VAR_BOOL);
    opts.emplace("eb", VAR_BOOL);
    opts.emplace("errorfile", VAR_STRING);
    opts.emplace("ef", VAR_STRING);
    opts.emplace("errorformat", VAR_STRING);
    opts.emplace("efm", VAR_STRING);
    opts.emplace("eventignore", VAR_STRING);
    opts.emplace("ei", VAR_STRING);
    opts.emplace("expandtab", VAR_BOOL);
    opts.emplace("et", VAR_BOOL);
    opts.emplace("exrc", VAR_BOOL);
    opts.emplace("ex", VAR_BOOL);
    opts.emplace("fileencoding", VAR_STRING);
    opts.emplace("fenc", VAR_STRING);
    opts.emplace("fileencodings", VAR_STRING);
    opts.emplace("fencs", VAR_STRING);
    opts.emplace("fileformat", VAR_STRING);
    opts.emplace("ff", VAR_STRING);
    opts.emplace("fileformats", VAR_STRING);
    opts.emplace("ffs", VAR_STRING);
    opts.emplace("fileignorecase", VAR_BOOL);
    opts.emplace("fic", VAR_BOOL);
    opts.emplace("filetype", VAR_STRING);
    opts.emplace("ft", VAR_STRING);
    opts.emplace("fillchars", VAR_STRING);
    opts.emplace("fcs", VAR_STRING);
    opts.emplace("fixendofline", VAR_BOOL);
    opts.emplace("fixeol", VAR_BOOL);
    opts.emplace("foldclose", VAR_STRING);
    opts.emplace("fcl", VAR_STRING);
    opts.emplace("foldcolumn", VAR_STRING);
    opts.emplace("fdc", VAR_STRING);
    opts.emplace("foldenable", VAR_BOOL);
    opts.emplace("fen", VAR_BOOL);
    opts.emplace("foldexpr", VAR_STRING);
    opts.emplace("fde", VAR_STRING);
    opts.emplace("foldignore", VAR_STRING);
    opts.emplace("fdi", VAR_STRING);
    opts.emplace("foldlevel", VAR_NUMBER);
    opts.emplace("fdl", VAR_NUMBER);
    opts.emplace("foldlevelstart", VAR_NUMBER);
    opts.emplace("fdls", VAR_NUMBER);
    opts.emplace("foldmarker", VAR_STRING);
    opts.emplace("fmr", VAR_STRING);
    opts.emplace("foldmethod", VAR_STRING);
    opts.emplace("fdm", VAR_STRING);
    opts.emplace("foldminlines", VAR_NUMBER);
    opts.emplace("fml", VAR_NUMBER);
    opts.emplace("foldnestmax", VAR_NUMBER);
    opts.emplace("fdn", VAR_NUMBER);
    opts.emplace("foldopen", VAR_STRING);
    opts.emplace("fdo", VAR_STRING);
    opts.emplace("foldtext", VAR_STRING);
    opts.emplace("fdt", VAR_STRING);
    opts.emplace("formatexpr", VAR_STRING);
    opts.emplace("fex", VAR_STRING);
    opts.emplace("formatoptions", VAR_STRING);
    opts.emplace("fo", VAR_STRING);
    opts.emplace("formatlistpat", VAR_STRING);
    opts.emplace("flp", VAR_STRING);
    opts.emplace("formatprg", VAR_STRING);
    opts.emplace("fp", VAR_STRING);
    opts.emplace("fsync", VAR_BOOL);
    opts.emplace("fs", VAR_BOOL);
    opts.emplace("gdefault", VAR_BOOL);
    opts.emplace("gd", VAR_BOOL);
    opts.emplace("grepformat", VAR_STRING);
    opts.emplace("gfm", VAR_STRING);
    opts.emplace("grepprg", VAR_STRING);
    opts.emplace("gp", VAR_STRING);
    opts.emplace("guicursor", VAR_STRING);
    opts.emplace("gcr", VAR_STRING);
    opts.emplace("guifont", VAR_STRING);
    opts.emplace("gfn", VAR_STRING);
    opts.emplace("guifontwide", VAR_STRING);
    opts.emplace("gfw", VAR_STRING);
    opts.emplace("guioptions", VAR_STRING);
    opts.emplace("go", VAR_STRING);
    opts.emplace("guitablabel", VAR_STRING);
    opts.emplace("gtl", VAR_STRING);
    opts.emplace("guitabtooltip", VAR_STRING);
    opts.emplace("gtt", VAR_STRING);
    opts.emplace("helpfile", VAR_STRING);
    opts.emplace("hf", VAR_STRING);
    opts.emplace("helpheight", VAR_NUMBER);
    opts.emplace("hh", VAR_NUMBER);
    opts.emplace("helplang", VAR_STRING);
    opts.emplace("hlg", VAR_STRING);
    opts.emplace("hidden", VAR_BOOL);
    opts.emplace("hid", VAR_BOOL);
    opts.emplace("highlight", VAR_STRING);
    opts.emplace("hl", VAR_STRING);
    opts.emplace("history", VAR_NUMBER);
    opts.emplace("hi", VAR_NUMBER);
    opts.emplace("hkmap", VAR_BOOL);
    opts.emplace("hk", VAR_BOOL);
    opts.emplace("hkmapp", VAR_BOOL);
    opts.emplace("hkp", VAR_BOOL);
    opts.emplace("hlsearch", VAR_BOOL);
    opts.emplace("hls", VAR_BOOL);
    opts.emplace("icon", VAR_BOOL);
    opts.emplace("iconstring", VAR_STRING);
    opts.emplace("ignorecase", VAR_BOOL);
    opts.emplace("ic", VAR_BOOL);
    opts.emplace("imcmdline", VAR_BOOL);
    opts.emplace("imc", VAR_BOOL);
    opts.emplace("imdisable", VAR_BOOL);
    opts.emplace("imd", VAR_BOOL);
    opts.emplace("iminsert", VAR_NUMBER);
    opts.emplace("imi", VAR_NUMBER);
    opts.emplace("imsearch", VAR_NUMBER);
    opts.emplace("ims", VAR_NUMBER);
    opts.emplace("inccommand", VAR_STRING);
    opts.emplace("icm", VAR_STRING);
    opts.emplace("include", VAR_STRING);
    opts.emplace("inc", VAR_STRING);
    opts.emplace("includeexpr", VAR_STRING);
    opts.emplace("inex", VAR_STRING);
    opts.emplace("incsearch", VAR_BOOL);
    opts.emplace("is", VAR_BOOL);
    opts.emplace("indentexpr", VAR_STRING);
    opts.emplace("inde", VAR_STRING);
    opts.emplace("indentkeys", VAR_STRING);
    opts.emplace("indk", VAR_STRING);
    opts.emplace("infercase", VAR_BOOL);
    opts.emplace("inf", VAR_BOOL);
    opts.emplace("insertmode", VAR_BOOL);
    opts.emplace("im", VAR_BOOL);
    opts.emplace("isfname", VAR_STRING);
    opts.emplace("isf", VAR_STRING);
    opts.emplace("isident", VAR_STRING);
    opts.emplace("isi", VAR_STRING);
    opts.emplace("iskeyword", VAR_STRING);
    opts.emplace("isk", VAR_STRING);
    opts.emplace("isprint", VAR_STRING);
    opts.emplace("isp", VAR_STRING);
    opts.emplace("joinspaces", VAR_BOOL);
    opts.emplace("js", VAR_BOOL);
    opts.emplace("jumpoptions", VAR_STRING);
    opts.emplace("jop", VAR_STRING);
    opts.emplace("keymap", VAR_STRING);
    opts.emplace("kmp", VAR_STRING);
    opts.emplace("keymodel", VAR_STRING);
    opts.emplace("km", VAR_STRING);
    opts.emplace("keywordprg", VAR_STRING);
    opts.emplace("kp", VAR_STRING);
    opts.emplace("langmap", VAR_STRING);
    opts.emplace("lmap", VAR_STRING);
    opts.emplace("langmenu", VAR_STRING);
    opts.emplace("lm", VAR_STRING);
    opts.emplace("langnoremap", VAR_BOOL);
    opts.emplace("lnr", VAR_BOOL);
    opts.emplace("langremap", VAR_BOOL);
    opts.emplace("lrm", VAR_BOOL);
    opts.emplace("laststatus", VAR_NUMBER);
    opts.emplace("ls", VAR_NUMBER);
    opts.emplace("lazyredraw", VAR_BOOL);
    opts.emplace("lz", VAR_BOOL);
    opts.emplace("linebreak", VAR_BOOL);
    opts.emplace("lbr", VAR_BOOL);
    opts.emplace("lines", VAR_NUMBER);
    opts.emplace("linespace", VAR_NUMBER);
    opts.emplace("lsp", VAR_NUMBER);
    opts.emplace("lisp", VAR_BOOL);
    opts.emplace("lispoptions", VAR_STRING);
    opts.emplace("lop", VAR_STRING);
    opts.emplace("lispwords", VAR_STRING);
    opts.emplace("lw", VAR_STRING);
    opts.emplace("list", VAR_BOOL);
    opts.emplace("listchars", VAR_STRING);
    opts.emplace("lcs", VAR_STRING);
    opts.emplace("loadplugins", VAR_BOOL);
    opts.emplace("lpl", VAR_BOOL);
    opts.emplace("magic", VAR_BOOL);
    opts.emplace("makeef", VAR_STRING);
    opts.emplace("mef", VAR_STRING);
    opts.emplace("makeencoding", VAR_STRING);
    opts.emplace("menc", VAR_STRING);
    opts.emplace("makeprg", VAR_STRING);
    opts.emplace("mp", VAR_STRING);
    opts.emplace("matchpairs", VAR_STRING);
    opts.emplace("mps", VAR_STRING);
    opts.emplace("matchtime", VAR_NUMBER);
    opts.emplace("mat", VAR_NUMBER);
    opts.emplace("maxcombine", VAR_NUMBER);
    opts.emplace("mco", VAR_NUMBER);
    opts.emplace("maxfuncdepth", VAR_NUMBER);
    opts.emplace("mfd", VAR_NUMBER);
    opts.emplace("maxmapdepth", VAR_NUMBER);
    opts.emplace("mmd", VAR_NUMBER);
    opts.emplace("maxmempattern", VAR_NUMBER);
    opts.emplace("mmp", VAR_NUMBER);
    opts.emplace("menuitems", VAR_NUMBER);
    opts.emplace("mis", VAR_NUMBER);
    opts.emplace("mkspellmem", VAR_STRING);
    opts.emplace("msm", VAR_STRING);
    opts.emplace("modeline", VAR_BOOL);
    opts.emplace("ml", VAR_BOOL);
    opts.emplace("modelineexpr", VAR_BOOL);
    opts.emplace("mle", VAR_BOOL);
    opts.emplace("modelines", VAR_NUMBER);
    opts.emplace("mls", VAR_NUMBER);
    opts.emplace("modifiable", VAR_BOOL);
    opts.emplace("ma", VAR_BOOL);
    opts.emplace("modified", VAR_BOOL);
    opts.emplace("mod", VAR_BOOL);
    opts.emplace("more", VAR_BOOL);
    opts.emplace("mouse", VAR_STRING);
    opts.emplace("mousefocus", VAR_BOOL);
    opts.emplace("mousef", VAR_BOOL);
    opts.emplace("mousehide", VAR_BOOL);
    opts.emplace("mh", VAR_BOOL);
    opts.emplace("mousemodel", VAR_STRING);
    opts.emplace("mousem", VAR_STRING);
    opts.emplace("mousemoveevent", VAR_BOOL);
    opts.emplace("mousemev", VAR_BOOL);
    opts.emplace("mousescroll", VAR_STRING);
    opts.emplace("mouseshape", VAR_STRING);
    opts.emplace("mouses", VAR_STRING);
    opts.emplace("mousetime", VAR_NUMBER);
    opts.emplace("mouset", VAR_NUMBER);
    opts.emplace("nrformats", VAR_STRING);
    opts.emplace("nf", VAR_STRING);
    opts.emplace("number", VAR_BOOL);
    opts.emplace("nu", VAR_BOOL);
    opts.emplace("numberwidth", VAR_NUMBER);
    opts.emplace("nuw", VAR_NUMBER);
    opts.emplace("omnifunc", VAR_STRING);
    opts.emplace("ofu", VAR_STRING);
    opts.emplace("opendevice", VAR_BOOL);
    opts.emplace("odev", VAR_BOOL);
    opts.emplace("operatorfunc", VAR_STRING);
    opts.emplace("opfunc", VAR_STRING);
    opts.emplace("packpath", VAR_STRING);
    opts.emplace("pp", VAR_STRING);
    opts.emplace("paragraphs", VAR_STRING);
    opts.emplace("para", VAR_STRING);
    opts.emplace("paste", VAR_BOOL);
    opts.emplace("pastetoggle", VAR_STRING);
    opts.emplace("pt", VAR_STRING);
    opts.emplace("patchexpr", VAR_STRING);
    opts.emplace("pex", VAR_STRING);
    opts.emplace("patchmode", VAR_STRING);
    opts.emplace("pm", VAR_STRING);
    opts.emplace("path", VAR_STRING);
    opts.emplace("pa", VAR_STRING);
    opts.emplace("preserveindent", VAR_BOOL);
    opts.emplace("pi", VAR_BOOL);
    opts.emplace("previewheight", VAR_NUMBER);
    opts.emplace("pvh", VAR_NUMBER);
    opts.emplace("previewwindow", VAR_BOOL);
    opts.emplace("pvw", VAR_BOOL);
    opts.emplace("prompt", VAR_BOOL);
    opts.emplace("pumblend", VAR_NUMBER);
    opts.emplace("pb", VAR_NUMBER);
    opts.emplace("pumheight", VAR_NUMBER);
    opts.emplace("ph", VAR_NUMBER);
    opts.emplace("pumwidth", VAR_NUMBER);
    opts.emplace("pw", VAR_NUMBER);
    opts.emplace("pyxversion", VAR_NUMBER);
    opts.emplace("pyx", VAR_NUMBER);
    opts.emplace("quickfixtextfunc", VAR_STRING);
    opts.emplace("qftf", VAR_STRING);
    opts.emplace("quoteescape", VAR_STRING);
    opts.emplace("qe", VAR_STRING);
    opts.emplace("readonly", VAR_BOOL);
    opts.emplace("ro", VAR_BOOL);
    opts.emplace("redrawdebug", VAR_STRING);
    opts.emplace("rdb", VAR_STRING);
    opts.emplace("redrawtime", VAR_NUMBER);
    opts.emplace("rdt", VAR_NUMBER);
    opts.emplace("regexpengine", VAR_NUMBER);
    opts.emplace("re", VAR_NUMBER);
    opts.emplace("relativenumber", VAR_BOOL);
    opts.emplace("rnu", VAR_BOOL);
    opts.emplace("remap", VAR_BOOL);
    opts.emplace("report", VAR_NUMBER);
    opts.emplace("revins", VAR_BOOL);
    opts.emplace("ri", VAR_BOOL);
    opts.emplace("rightleft", VAR_BOOL);
    opts.emplace("rl", VAR_BOOL);
    opts.emplace("rightleftcmd", VAR_STRING);
    opts.emplace("rlc", VAR_STRING);
    opts.emplace("ruler", VAR_BOOL);
    opts.emplace("ru", VAR_BOOL);
    opts.emplace("rulerformat", VAR_STRING);
    opts.emplace("ruf", VAR_STRING);
    opts.emplace("runtimepath", VAR_STRING);
    opts.emplace("rtp", VAR_STRING);
    opts.emplace("scroll", VAR_NUMBER);
    opts.emplace("scr", VAR_NUMBER);
    opts.emplace("scrollback", VAR_NUMBER);
    opts.emplace("scbk", VAR_NUMBER);
    opts.emplace("scrollbind", VAR_BOOL);
    opts.emplace("scb", VAR_BOOL);
    opts.emplace("scrolljump", VAR_NUMBER);
    opts.emplace("sj", VAR_NUMBER);
    opts.emplace("scrolloff", VAR_NUMBER);
    opts.emplace("so", VAR_NUMBER);
    opts.emplace("scrollopt", VAR_STRING);
    opts.emplace("sbo", VAR_STRING);
    opts.emplace("sections", VAR_STRING);
    opts.emplace("sect", VAR_STRING);
    opts.emplace("secure", VAR_BOOL);
    opts.emplace("selection", VAR_STRING);
    opts.emplace("sel", VAR_STRING);
    opts.emplace("selectmode", VAR_STRING);
    opts.emplace("slm", VAR_STRING);
    opts.emplace("sessionoptions", VAR_STRING);
    opts.emplace("ssop", VAR_STRING);
    opts.emplace("shada", VAR_STRING);
    opts.emplace("sd", VAR_STRING);
    opts.emplace("shadafile", VAR_STRING);
    opts.emplace("sdf", VAR_STRING);
    opts.emplace("shell", VAR_STRING);
    opts.emplace("sh", VAR_STRING);
    opts.emplace("shellcmdflag", VAR_STRING);
    opts.emplace("shcf", VAR_STRING);
    opts.emplace("shellpipe", VAR_STRING);
    opts.emplace("sp", VAR_STRING);
    opts.emplace("shellquote", VAR_STRING);
    opts.emplace("shq", VAR_STRING);
    opts.emplace("shellredir", VAR_STRING);
    opts.emplace("srr", VAR_STRING);
    opts.emplace("shellslash", VAR_BOOL);
    opts.emplace("ssl", VAR_BOOL);
    opts.emplace("shelltemp", VAR_BOOL);
    opts.emplace("stmp", VAR_BOOL);
    opts.emplace("shellxquote", VAR_STRING);
    opts.emplace("sxq", VAR_STRING);
    opts.emplace("shellxescape", VAR_STRING);
    opts.emplace("sxe", VAR_STRING);
    opts.emplace("shiftround", VAR_BOOL);
    opts.emplace("sr", VAR_BOOL);
    opts.emplace("shiftwidth", VAR_NUMBER);
    opts.emplace("sw", VAR_NUMBER);
    opts.emplace("shortmess", VAR_STRING);
    opts.emplace("shm", VAR_STRING);
    opts.emplace("showbreak", VAR_STRING);
    opts.emplace("sbr", VAR_STRING);
    opts.emplace("showcmd", VAR_BOOL);
    opts.emplace("sc", VAR_BOOL);
    opts.emplace("showcmdloc", VAR_STRING);
    opts.emplace("sloc", VAR_STRING);
    opts.emplace("showfulltag", VAR_BOOL);
    opts.emplace("sft", VAR_BOOL);
    opts.emplace("showmatch", VAR_BOOL);
    opts.emplace("sm", VAR_BOOL);
    opts.emplace("showmode", VAR_BOOL);
    opts.emplace("smd", VAR_BOOL);
    opts.emplace("showtabline", VAR_NUMBER);
    opts.emplace("stal", VAR_NUMBER);
    opts.emplace("sidescroll", VAR_NUMBER);
    opts.emplace("ss", VAR_NUMBER);
    opts.emplace("sidescrolloff", VAR_NUMBER);
    opts.emplace("siso", VAR_NUMBER);
    opts.emplace("signcolumn", VAR_STRING);
    opts.emplace("scl", VAR_STRING);
    opts.emplace("smartcase", VAR_BOOL);
    opts.emplace("scs", VAR_BOOL);
    opts.emplace("smartindent", VAR_BOOL);
    opts.emplace("si", VAR_BOOL);
    opts.emplace("smarttab", VAR_BOOL);
    opts.emplace("sta", VAR_BOOL);
    opts.emplace("softtabstop", VAR_NUMBER);
    opts.emplace("sts", VAR_NUMBER);
    opts.emplace("spell", VAR_BOOL);
    opts.emplace("spellcapcheck", VAR_STRING);
    opts.emplace("spc", VAR_STRING);
    opts.emplace("spellfile", VAR_STRING);
    opts.emplace("spf", VAR_STRING);
    opts.emplace("spelllang", VAR_STRING);
    opts.emplace("spl", VAR_STRING);
    opts.emplace("spellsuggest", VAR_STRING);
    opts.emplace("sps", VAR_STRING);
    opts.emplace("spelloptions", VAR_STRING);
    opts.emplace("spo", VAR_STRING);
    opts.emplace("splitbelow", VAR_BOOL);
    opts.emplace("sb", VAR_BOOL);
    opts.emplace("splitkeep", VAR_STRING);
    opts.emplace("spk", VAR_STRING);
    opts.emplace("splitright", VAR_BOOL);
    opts.emplace("spr", VAR_BOOL);
    opts.emplace("startofline", VAR_BOOL);
    opts.emplace("sol", VAR_BOOL);
    opts.emplace("statuscolumn", VAR_STRING);
    opts.emplace("stc", VAR_STRING);
    opts.emplace("statusline", VAR_STRING);
    opts.emplace("stl", VAR_STRING);
    opts.emplace("suffixes", VAR_STRING);
    opts.emplace("su", VAR_STRING);
    opts.emplace("suffixesadd", VAR_STRING);
    opts.emplace("sua", VAR_STRING);
    opts.emplace("swapfile", VAR_BOOL);
    opts.emplace("swf", VAR_BOOL);
    opts.emplace("switchbuf", VAR_STRING);
    opts.emplace("swb", VAR_STRING);
    opts.emplace("synmaxcol", VAR_NUMBER);
    opts.emplace("smc", VAR_NUMBER);
    opts.emplace("syntax", VAR_STRING);
    opts.emplace("syn", VAR_STRING);
    opts.emplace("tagfunc", VAR_STRING);
    opts.emplace("tfu", VAR_STRING);
    opts.emplace("tabline", VAR_STRING);
    opts.emplace("tal", VAR_STRING);
    opts.emplace("tabpagemax", VAR_NUMBER);
    opts.emplace("tpm", VAR_NUMBER);
    opts.emplace("tabstop", VAR_NUMBER);
    opts.emplace("ts", VAR_NUMBER);
    opts.emplace("tagbsearch", VAR_BOOL);
    opts.emplace("tbs", VAR_BOOL);
    opts.emplace("tagcase", VAR_STRING);
    opts.emplace("tc", VAR_STRING);
    opts.emplace("taglength", VAR_NUMBER);
    opts.emplace("tl", VAR_NUMBER);
    opts.emplace("tagrelative", VAR_BOOL);
    opts.emplace("tr", VAR_BOOL);
    opts.emplace("tags", VAR_STRING);
    opts.emplace("tag", VAR_STRING);
    opts.emplace("tagstack", VAR_BOOL);
    opts.emplace("tgst", VAR_BOOL);
    opts.emplace("termbidi", VAR_BOOL);
    opts.emplace("tbidi", VAR_BOOL);
    opts.emplace("termencoding", VAR_STRING);
    opts.emplace("tenc", VAR_STRING);
    opts.emplace("termguicolors", VAR_BOOL);
    opts.emplace("tgc", VAR_BOOL);
    opts.emplace("termpastefilter", VAR_STRING);
    opts.emplace("tpf", VAR_STRING);
    opts.emplace("terse", VAR_BOOL);
    opts.emplace("textwidth", VAR_NUMBER);
    opts.emplace("tw", VAR_NUMBER);
    opts.emplace("thesaurus", VAR_STRING);
    opts.emplace("tsr", VAR_STRING);
    opts.emplace("thesaurusfunc", VAR_STRING);
    opts.emplace("tsrfu", VAR_STRING);
    opts.emplace("tildeop", VAR_BOOL);
    opts.emplace("top", VAR_BOOL);
    opts.emplace("timeout", VAR_BOOL);
    opts.emplace("to", VAR_BOOL);
    opts.emplace("timeoutlen", VAR_NUMBER);
    opts.emplace("tm", VAR_NUMBER);
    opts.emplace("title", VAR_BOOL);
    opts.emplace("titlelen", VAR_NUMBER);
    opts.emplace("titleold", VAR_STRING);
    opts.emplace("titlestring", VAR_STRING);
    opts.emplace("ttimeout", VAR_BOOL);
    opts.emplace("ttimeoutlen", VAR_NUMBER);
    opts.emplace("ttm", VAR_NUMBER);
    opts.emplace("ttyfast", VAR_BOOL);
    opts.emplace("tf", VAR_BOOL);
    opts.emplace("undodir", VAR_STRING);
    opts.emplace("udir", VAR_STRING);
    opts.emplace("undofile", VAR_BOOL);
    opts.emplace("udf", VAR_BOOL);
    opts.emplace("undolevels", VAR_NUMBER);
    opts.emplace("ul", VAR_NUMBER);
    opts.emplace("undoreload", VAR_NUMBER);
    opts.emplace("ur", VAR_NUMBER);
    opts.emplace("updatecount", VAR_NUMBER);
    opts.emplace("uc", VAR_NUMBER);
    opts.emplace("updatetime", VAR_NUMBER);
    opts.emplace("ut", VAR_NUMBER);
    opts.emplace("varsofttabstop", VAR_STRING);
    opts.emplace("vsts", VAR_STRING);
    opts.emplace("vartabstop", VAR_STRING);
    opts.emplace("vts", VAR_STRING);
    opts.emplace("verbose", VAR_NUMBER);
    opts.emplace("vbs", VAR_NUMBER);
    opts.emplace("verbosefile", VAR_STRING);
    opts.emplace("vfile", VAR_STRING);
    opts.emplace("viewdir", VAR_STRING);
    opts.emplace("vdir", VAR_STRING);
    opts.emplace("viewoptions", VAR_STRING);
    opts.emplace("vop", VAR_STRING);
    opts.emplace("viminfo", VAR_STRING);
    opts.emplace("vi", VAR_STRING);
    opts.emplace("viminfofile", VAR_STRING);
    opts.emplace("vif", VAR_STRING);
    opts.emplace("virtualedit", VAR_STRING);
    opts.emplace("ve", VAR_STRING);
    opts.emplace("visualbell", VAR_BOOL);
    opts.emplace("vb", VAR_BOOL);
    opts.emplace("warn", VAR_BOOL);
    opts.emplace("whichwrap", VAR_STRING);
    opts.emplace("ww", VAR_STRING);
    opts.emplace("wildchar", VAR_NUMBER);
    opts.emplace("wc", VAR_NUMBER);
    opts.emplace("wildcharm", VAR_NUMBER);
    opts.emplace("wcm", VAR_NUMBER);
    opts.emplace("wildignore", VAR_STRING);
    opts.emplace("wig", VAR_STRING);
    opts.emplace("wildignorecase", VAR_BOOL);
    opts.emplace("wic", VAR_BOOL);
    opts.emplace("wildmenu", VAR_BOOL);
    opts.emplace("wmnu", VAR_BOOL);
    opts.emplace("wildmode", VAR_STRING);
    opts.emplace("wim", VAR_STRING);
    opts.emplace("wildoptions", VAR_STRING);
    opts.emplace("wop", VAR_STRING);
    opts.emplace("winaltkeys", VAR_STRING);
    opts.emplace("wak", VAR_STRING);
    opts.emplace("winbar", VAR_STRING);
    opts.emplace("wbr", VAR_STRING);
    opts.emplace("winblend", VAR_NUMBER);
    opts.emplace("winbl", VAR_NUMBER);
    opts.emplace("winhighlight", VAR_STRING);
    opts.emplace("winhl", VAR_STRING);
    opts.emplace("window", VAR_NUMBER);
    opts.emplace("wi", VAR_NUMBER);
    opts.emplace("winheight", VAR_NUMBER);
    opts.emplace("wh", VAR_NUMBER);
    opts.emplace("winfixheight", VAR_BOOL);
    opts.emplace("wfh", VAR_BOOL);
    opts.emplace("winfixwidth", VAR_BOOL);
    opts.emplace("wfw", VAR_BOOL);
    opts.emplace("winminheight", VAR_NUMBER);
    opts.emplace("wmh", VAR_NUMBER);
    opts.emplace("winminwidth", VAR_NUMBER);
    opts.emplace("wmw", VAR_NUMBER);
    opts.emplace("winwidth", VAR_NUMBER);
    opts.emplace("wiw", VAR_NUMBER);
    opts.emplace("wrap", VAR_BOOL);
    opts.emplace("wrapmargin", VAR_NUMBER);
    opts.emplace("wm", VAR_NUMBER);
    opts.emplace("wrapscan", VAR_BOOL);
    opts.emplace("ws", VAR_BOOL);
    opts.emplace("write", VAR_BOOL);
    opts.emplace("writeany", VAR_BOOL);
    opts.emplace("wa", VAR_BOOL);
    opts.emplace("writebackup", VAR_BOOL);
    opts.emplace("wb", VAR_BOOL);
    opts.emplace("writedelay", VAR_NUMBER);
    opts.emplace("wd", VAR_NUMBER);

    for (int i = 0; i < 32; ++i) {
        quickTab[i] = opts.count();
    }
    for (int i = 1; i < opts.count(); ++i) {
        int quickIdx = opts.map[i].key[0] - 'a';
        if (i < quickTab[quickIdx]) {
            quickTab[quickIdx] = i;
        }
    }
}
