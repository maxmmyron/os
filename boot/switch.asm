[bits 16]
switch_pm:
  cli                     ; disable interrupts
  lgdt  [gdt_descriptor]  ; load GDT descriptor
                          ; we want to change the last bit of cr0 (the control
                          ; register) to 1, to change the CPU from real mode to
                          ; protected mode. we can't directly edit the cr0
                          ; register, so what we do is:
  mov   eax, cr0          ; 1.  copy the register into a 32-bit general-purpose
                          ;     register
  or    eax, 0x1          ; 2.  change the last bit to 1
  mov   cr0, eax          ; 3.  copy the result back into cr0.
  jmp   CODE_SEG:init_pm  ; perform a far-jump, which is a jump into a different
                          ; segment. we need to specify an actual segment for
                          ; a far jump which is the purpose of the "CODE_SEG"
                          ; before the offset "init_pm"

[bits 32]                 ; at this point, we're in 32 bit mode, so we need to
                          ; specify that explicitly so we can take advantage of
                          ; 32 bit instructions.
init_pm:
  mov   ax, DATA_SEG      ; update segment registers
  mov   ds, ax
  mov   ss, ax
  mov   es, ax
  mov   fs, ax
  mov   gs, ax

  mov   ebp, 0x90000      ; update the stack
  mov   esp, ebp

  call  begin_pm          ; start running useful 32-bit code
