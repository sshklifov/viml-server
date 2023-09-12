function! ale#ant#FindProjectRoot(buffer) abort
    let l:build_xml_path = ale#path#FindNearestFile(a:buffer, 'build.xml')

    if !empty(l:build_xml_path)
        return fnamemodify(l:build_xml_path, ':h')
    endif

    return ''
endfunction
