[org 0x7c00]

main:
  mov bx, os
  call print
  call print_nl

  mov bx, ver
  call print
  call print_nl

  jmp   end_loop

end_loop:
  pause
  jmp   end_loop

%include "boot_pstr.asm"

os:
  db "os", 0
ver:
  db "v0.0", 0

times   510 - ($-$$) db 0 ; create n zeros, where n is 510 (512 - 2 magic number
                          ; bytes at end) minus the size of the previous code
dw      0xaa55            ; magic number that confirms this code is bootable
