#include "kernel.h"
#include "../libc/str.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../libc/mem.h"

int proc() {
  print("process running...\n");
  return 0;
}

void kernel_main() {
  set_screen_attr(WHITE_ON_BLACK);
  clear_screen();
  print("> ");

  isr_install();
  irq_install();

  process_table = malloc(sizeof(struct pcb*) * MAX_PROCESSES);
  // we allocate 256 * pcb pointer size (~1024 bytes) towards process table

  char str[32];
  itoa((int)process_table, str);
  print("process_table malloc'd at mem addr: ");
  print(str);
  print("\n");


  itoa(sizeof(struct pcb*) * MAX_PROCESSES, str);
  print("sz malloc'd: ");
  print(str);
  print("\n");

  int i = 0;
  for(i = 0; i < MAX_PROCESSES; i++)
    process_table[i] = 0x00;  // init each value to null value

  // set up process
  struct pcb *p = malloc(sizeof(*p));

  itoa((int)p, str);
  print("pcb malloc'd at mem addr: ");
  print(str);
  print("\n");

  p->name = "Process";
  p->function = proc;

  process_table[0] = p;

  print(process_table[0]->name);
  print("\n");
  process_table[0]->function();
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
