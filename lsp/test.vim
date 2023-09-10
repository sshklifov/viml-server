function! Delete(bang)
  let a = input("ARE YOU SURE??? ")
  redraw
  if a 'y'
    echo "Aborted"
    return
  endif
  try
    let file = expand("%:p")
    exe "bw" . a:bang
    call delete(file)
  catch
    echoerr "No write since last change. Add ! to override."
  endtry
endfunction

command! -nargs=0 -bang Delete call Delete('<bang>')
