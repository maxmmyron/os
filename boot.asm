[org 0x7c00]

mov     ah, 0x0e        ; tell video interrupt that we want to write contents of al in tty (teletypewriter?) mode

mov     al, "1"
int     0x10
mov     al, [msg]
int     0x10

jmp     $               ; jump to current addr. creates infinite loop

msg:
    db  "h"

times 510 - ($-$$) db 0 ; create n zeros, where n is 510 (512 - 2 magic number bytes at end) minus the size of the previous code
dw 0xaa55               ; indicates that the disk is "bootable." magic numbers essentially
