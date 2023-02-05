#include "kernel.h"
#include "../process/k_process.h"
#include "../drivers/screen.h"
#include "../clib/str.h"

void kernel_main() {
    // clear screen after boot process completes
    clear_screen();

    // fill screen
    int i = 0;
    for(i = 0; i < 24; i++) {
        char str[255];
        itoa(i, str);
        print_str_at(str, 0, i);
    }
}
