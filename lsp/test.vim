let l:column = searchpos('\V' . escape(l:word, '/\'), 'bnc', l:line)[1]

let g:should_complete_map = {
\   '<default>': '\v[a-zA-Z$_][a-zA-Z$_0-9]*$|\.$',
\   'racket': '\k\+$',
\   'typescript': '\v[a-zA-Z$_][a-zA-Z$_0-9]*$|\.$|''$|"$',
\   'rust': '\v[a-zA-Z$_][a-zA-Z$_0-9]*$|\.$|::$',
\   'cpp': '\v[a-zA-Z$_][a-zA-Z$_0-9]*$|\.$|::$|-\>$',
\   'c': '\v[a-zA-Z$_][a-zA-Z$_0-9]*$|\.$|-\>$',
\}
