[org 0x7c00]

start:
  mov   si, message   ; load the source index register with the address of msg
  call  print_string  ; print string
  mov   si, nstr      ; load with new string
  jmp   main

print_string:
  mov   ah, 0x0e      ; set the function code to 0Eh, which prints a string
.print_loop:
  lodsb               ; load the byte in si and increment

  cmp   al, 0x00      ; compare al to 0 (the null terminator)
  je    .done         ; if ZL is high (prev cmp is true) then jump to done

  int   10h           ; otherwise, print the character in al
  jmp   .print_loop
.done:
  ret                 ; return from call

main:
  mov   bp, [0x8000]  ; point address of current stack frame to 0x8000, which is
                      ; far enough from our boot segement that we can be confident
                      ; it won't be overwritten.
  mov   sp, bp        ; point the stack pointer to the bsae pointer since the
                      ; stack is empty

  push  'A'           ; push to the stack
  push  'B'
  push  'C'

  mov   al, [0x7ffe]  ; the stack grows downwards, so the top of the stack is
                      ; accessible 2 bytes away from 0x8000
  int   10h
  mov   al, [0x8000]  ; we cannot access this because we can only access the stack
                      ; from the top (0x7ffe)
  int   10h

  pop   bx            ; pop from the stack into the bx register. by defalt, pop
                      ; returns a 16 byte word, so we need to utilize the bx
                      ; register (since we only want one byte at a time in al)
  mov   al, bl        ; move the lower 8 bits of bx into al (i.e. the first byte
                      ; of the stack)
  int   10h

  pop   bx
  mov   al, bl
  int   10h

  pop   bx
  mov   al, bl
  int   10h

  jmp   end_loop

end_loop:
  pause
  jmp   end_loop

message db "os", 0xa, 0xd, 0
nstr    db "v0.0", 0xa, 0xd, 0

times   510 - ($-$$) db 0 ; create n zeros, where n is 510 (512 - 2 magic number bytes at end) minus the size of the previous code
dw      0xaa55
