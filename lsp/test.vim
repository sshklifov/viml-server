for l:key in filter(keys(g:), l:filter_expr)
    execute 'Save g:' . l:key
    unlet g:[l:key]
endfor
