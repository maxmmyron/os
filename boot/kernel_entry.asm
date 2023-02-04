; this routine attaches to the start of the kernel machine code (main() in
; kernel.c). we use assembly since we know exactly how this will be translated
; into machine code, and we can make sure that main() is the first function
; called.

[bits 32]       ; use 32-bit instructions, since by this point we've loaded into
                ; protected mode.
[extern main]   ; define the calling point as an external symbol. the linker
                ; will substitute with the final address

call  main      ; invoke main() in the C kernel
                ; the main label is resolved during link time. our [extern main]
                ; line tells the linker we expect to find the main label in a
                ; different object file (kernel.c())

jmp   $         ; if we ever get back control from the kernel, we can just hang
