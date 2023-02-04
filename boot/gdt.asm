gdt_start:
  ; GDT always starts with 8 bytes null
  dd  0x0
  dd  0x0

gdt_code:
  dw  0xffff    ; segment length          bits 0  - 15
  dw  0x0       ; segment base            bits 0  - 15
  db  0x0       ; segment base            bits 15 - 23
  db  10011010b ; flags (8bit)
  db  11001111b ; flags (4bit)            bits 16 - 19
  db  0x0       ; segment base            bits 24 - 31

gdt_data:
  dw  0xffff
  dw  0x0
  db  0x0
  db  10010010b
  db  11001111b
  db  0x0

gdt_end:

; we can't directly load the GDT, so we define a gdt_descriptor that contains
; the size and address of the GDT. we then load the gdt using the lgdt operation
gdt_descriptor:
  dw gdt_end - gdt_start - 1    ; 16-bit size (one less than true size)
  dd gdt_start                  ; 32-bit address

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
