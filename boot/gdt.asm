; define the start of the GDT. the first entry (8 bytes) should always be null,
; so we will define two double words (i.e. 8 bytes) to 0
gdt_start:
  dd  0x0
  dd  0x0

gdt_code:
  dw  0xffff    ; limit           bits 0  - 15  (0  - 15)
  dw  0x0       ; base            bits 0  - 15  (16 - 31)
  db  0x0       ; base            bits 16 - 23  (32 - 39)
  db  10011010b ; access byte     (8bit)        (40 - 47)
                ; the higher four bits above describe the following:
                ; present:  whether or not the segment is used. if 0, generates
                ;           a "segment not present" exception.
                ; privlege: 2 bits; denotes privlege of segment from 0-3. lower
                ;           privlege values denote a higher overall privlege,
                ;           with 0 (kernel) being highest
                ; type:     the descriptor bit type. if zero, the descriptor
                ;           describes a system segment. if one, the descriptor
                ;           describes a data or code segment.

                ; the lower four bits are determined first by the type bit.
                ; if the type bit is one (i.e. data/code segment):
                ;   exec bit:
                ;   C/D bit:
                ;     for code segments:  conforming bit. if 0, the segment can
                ;                         only be executed from the ring set in
                ;                         DPL (i.e. equal privledge). if 1, then
                ;                         code in this segment can be executed
                ;                         by equal *or lower* privlege levels.
                ;                         (e.g. code in ring 3 can far-jump to
                ;                         conforming code in ring 2.)
                ;     for data segments:  direction bit. if 0, the segments
                ;                         grow upwards. if 1, the segments grow
                ;                         downwards; offset > limit.
                ;   R/W bit:
                ;     for code segments:  readable bit. if 0, read access not
                ;                         allowed. write access is *never*
                ;                         allowed for code segments.
                ;     for data segments:  writeable bit. if 0, write access not
                ;                         allowed. read access is *always*
                ;                         allowed for data segments.
                ;   accessed bit:   standard to leave this clear
  db  11001111b ; limit           bits 16 - 19  (48 - 51)
                ; flags           (4bit)        (52 - 55)
                ; the lower four bits in this byte describe the remainder of the
                ; 20-bit limit value
                ; the higher four bits in this byte describe a few flags related
                ; to address sizing:
                ; granularity:  indicates the size by which the limit value is
                ;               scaled by.
                ;               if 0, the limit's unit value is 1 byte blocks,
                ;               for a maximum size of 2^20 bytes.
                ;               if 1, the limit's unit value is 4096 byte blocks
                ;               for a maximum size of 2^32 bytes.
                ; size:         if 0, descriptor defines a 16-bit code segment.
                ;               if 1, defines a 32-bit protected mode segment.
                ;               A GDT can have both 16-bit and 32-bit selectors
                ;               at the same time for real and protected mode
                ;               respectively.
                ; long-mode:    if 1, the descriptor defines a 64-bit code
                ;               segment. if set, size flag should always be
                ;               clear.
  db  0x0       ; segment base    bits 24 - 31  (56 - 63)

; define a GDT descriptor for data segment (just like we did with the code
; segment)
gdt_data:
  dw  0xffff
  dw  0x0
  db  0x0
  db  10010010b
  db  11001111b
  db  0x0

; take note of the end of the GDT in memory so we can calculate the size of the
; GDT segment
gdt_end:

; we can't directly load the GDT, so we define a gdt_descriptor that contains
; the size and address of the GDT. we then load the gdt using the lgdt operation
gdt_descriptor:
  dw gdt_end - gdt_start - 1    ; 16-bit size (one less than true size)
  dd gdt_start                  ; 32-bit address

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
