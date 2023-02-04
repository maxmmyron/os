#include "../drivers/screen.h"
#include "util.h"

void main() {
    // clear screen after boot process completes
    clear_screen();

    // fill screen
    int i = 0;
    for(i = 0; i < 24; i++) {
        char str[255];
        k_itoa(i, str);
        k_print_at(str, 0, i);
    }

    // k_print("\n");
    // k_print("Force the kernel to scroll; row 0 disappears\n");
    // k_print("Force the kernel to scroll again; row 1 disappears");
}
