[org 0x7c00]

main:
  mov   bp, 0x8000
  mov   sp, bp

  mov   bx, 0x9000  ;
  mov   dh, 2       ; read 2 sectors

  call disk_load

  mov   bx, os
  call  print
  call  print_nl

  mov bx, ver
  call print
  call print_nl

  mov dx, [0x9000]        ; get first word from sector 2
  call print_hex
  call print_nl

  mov dx, [0x9000 + 512]  ; get first word from sector 3
  call print_hex

  jmp   end_loop

end_loop:
  pause
  jmp   end_loop

%include "boot_pstr.asm"
%include "boot_phex.asm"
%include "boot_disk.asm"

os:
  db "os", 0
ver:
  db "v0.0", 0

times   510 - ($-$$) db 0 ; create n zeros, where n is 510 (512 - 2 magic number
                          ; bytes at end) minus the size of the previous code
dw      0xaa55            ; magic number that confirms this code is bootable

times 256 dw 0xbebe       ; sector 2: 512 bytes
times 250 dw 0x3639       ; sector 3: 512 bytes
