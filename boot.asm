[org 0x7c00]

mov si, message   ; load the source index register with the address of our message
call print_str

print_str:
  mov ah, 0Eh     ; set the function code to 0Eh. This is the function code for printing a string

.run:
  lodsb           ; load the byte at the source index into al and increment si
  cmp al, 0x00    ; compare al to 0 (the null terminator)
  je .done        ; if al is 0, jump to .done
  int 10h         ; otherwise, print the character in al
  jmp .run        ; and jump back to .run

.done:
  jmp $           ; infinte loop

message   db "Hello World!", 0

times 510 - ($-$$) db 0 ; create n zeros, where n is 510 (512 - 2 magic number bytes at end) minus the size of the previous code
dw 0xaa55
