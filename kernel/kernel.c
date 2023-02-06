#include "kernel.h"
#include "../libc/str.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"


void kernel_main() {
  set_screen_attr(WHITE_ON_BLACK);
  clear_screen();
  print("> ");

  isr_install();
  irq_install();
}

void user_input(char* input) {
  if(strcmp(input, "END") == 0) {
    print("shutdown\n");
    asm volatile("hlt");
  }

  if(strcmp(input, "TPANIC") == 0) {
    // test panic
    asm volatile("int $3");
  }

  print("command: ");
  print(input);
  print("\n> ");
}

// panics when interrupt received
void kernel_panic(char* exception_message) {
  set_screen_attr(BLACK_ON_RED);
  clear_screen();
  print("panic!\n");
  print("exception message: ");
  print(exception_message);
  asm volatile("hlt");
}
