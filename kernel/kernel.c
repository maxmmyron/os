// #include "kernel.h"
// #include "../process/k_process.h"
#include "../drivers/screen.h"
// #include "../clib/str.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"

void kernel_main() {
    // clear screen after boot process completes
    // clear_screen();

    // // fill screen
    // int i = 0;
    // for(i = 0; i < 24; i++) {
    //     char str[255];
    //     itoa(i, str);
    //     print_str_at(str, 0, i);
    // }

    isr_install();
    /* Test the interrupts */
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
}
