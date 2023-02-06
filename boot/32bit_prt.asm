[bits 32]

VIDEO_MEMORY  equ 0xb8000 ; in text mode, video memory starts specifically at
                          ; 0xb8000
WOB           equ 0x0f

pstr_pm:
  pusha
  mov   edx, VIDEO_MEMORY ; edx contains our video memory

pstr_pm_loop:
  mov al, [ebx]   ; move into al reg from address of character
  mov ah, WOB

  cmp al, 0
  je pstr_pm_done

  mov [edx], ax   ; store character and attribute in video memory
  add ebx, 1      ; move to next char
  add edx, 2      ; move to next video memory position

  jmp pstr_pm_loop

pstr_pm_done:
  popa
  ret
