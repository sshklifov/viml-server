let l:middle[-1] .= l:insertions[0]
let l:middle     += l:insertions[1:]
let l:middle[-1] .= l:lines[l:end_line - 1][l:end_column - 1 :]
