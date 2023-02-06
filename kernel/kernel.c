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

  if(strcmp(input, "TPANIC") == 0) {
    // test panic
    asm volatile("int $3");
  }

  print_str("command: ");
  print_str(input);
  print_str("\n> ");
}

// panics when interrupt received
void kernel_panic(char* exception_message) {
  clear_screen();
  print_str("panic\n");
  print_str(exception_message);
  asm volatile("hlt");
}
