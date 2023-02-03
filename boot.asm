[org 0x7c00]

mov   bp, 0x8000        ; set up stack
mov   sp, bp

mov   bx, 0x9000
mov   dh, 2             ; read 2 sectors

call  disk_load

begin_rl:
  mov   bx, msg_real
  call  print
  call  print_nl

  mov   bx, msg_os
  call  print
  call  print_nl

  mov   dx, [0x9000]        ; get first word from sector 2
  call  print_hex
  call  print_nl

  mov   dx, [0x9000 + 512]  ; get first word from sector 3
  call  print_hex

  call  switch_pm

%include "boot_pstr.asm"
%include "boot_phex.asm"
%include "boot_disk.asm"

%include "32bit_gdt.asm"
%include "32bit_pstr.asm"
%include "32bit_switch.asm"

[bits 32]
begin_pm:
  mov ebx, msg_prot ; move 32-bit protected message to extended bx register
                    ; (since that is 32 bits wide)
  call pstr_pm
  jmp $

end_loop:
  pause
  jmp   end_loop

msg_real:
  db "loaded 16-bit real mode", 0
msg_os:
  db "os v0", 0
msg_prot:
  db "loaded 32-bit protected mode", 0

times   510 - ($-$$) db 0 ; create n zeros, where n is 510 (512 - 2 magic number
                          ; bytes at end) minus the size of the previous code
dw      0xaa55            ; magic number that confirms this code is bootable

times 256 dw 0xbebe       ; sector 2: 512 bytes
times 250 dw 0x3639       ; sector 3: 512 bytes
