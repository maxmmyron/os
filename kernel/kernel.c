#include "kernel.h"
#include "../libc/str.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"


void kernel_main() {
  clear_screen();
  print_str("> ");

  isr_install();
  irq_install();
}

void user_input(char* input) {
  if(strcmp(input, "END") == 0) {
    print_str("shutdown\n");
    asm volatile("hlt");
  }

  print_str("command: ");
  print_str(input);
  print_str("\n> ");
}
