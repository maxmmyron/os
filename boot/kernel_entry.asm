[bits 32]
[extern main]   ; define calling point, which is same name as kernel.c main func

call  main      ; call the C function (linker knows where in mem is it)
jmp   $
