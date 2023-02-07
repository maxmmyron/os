[org 0x7c00]
KERNEL_OFFSET equ 0x1000  ; same value we used when linking kernel

section .text
_start:
mov   [boot_drive], dl    ; BIOS sets boot drive in dl on boot
mov   bp, 0x9000          ; set up stack
mov   sp, bp

mov   bx, msg_ld_real
call  print
call  print_nl

call  load_kernel         ; load kernel from disk
call  switch_pm           ; disable interrupts, load GDT, jump to begin_pm
.hang:
  hlt
  jmp .hang
.end:

; 16 bit includes
%include "boot/prt_str.asm"
%include "boot/prt_hex.asm"
%include "boot/disk.asm"

; 32 bit includes
%include "boot/gdt.asm"
%include "boot/32bit_prt.asm"
%include "boot/switch.asm"

[bits 16]
load_kernel:
  mov   bx, msg_ld_kernel
  call  print
  call  print_nl

  mov   bx, KERNEL_OFFSET
  mov   dh, 48
  mov   dl, [boot_drive]

  call  disk_load
  ret

[bits 32]
begin_pm:
  mov   ebx, msg_ld_prot  ; move 32-bit protected message to extended bx
                          ; register (since that is 32 bits wide)
  call  pstr_pm
  call  KERNEL_OFFSET     ; give control to kernel
  jmp   $                 ; once control returned from kernel, loop

boot_drive:
  db 0
msg_ld_real:
  db "loaded 16-bit real mode", 0
msg_ld_kernel:
  db "os v0... loading kernel into memory", 0
msg_ld_prot:
  db "loaded 32-bit protected mode", 0

times   510 - ($-$$) db 0 ; create n zeros, where n is 510 (512 - 2 magic number
                          ; bytes at end) minus the size of the previous code
dw      0xaa55            ; magic number that confirms this code is bootable
