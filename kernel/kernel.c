#include "kernel.h"
#include "../libc/str.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../libc/mem.h"
#include "../libc/function.h"

int p1v = 0;
int process1() {
  return ++p1v;
}

int p2v = 0;
int process2() {
  p2v += 2;
  return p2v;
}

// internal functions
void draw_process_table();
int create_process(char* name, int(*function)(void));

void kernel_main() {
  set_screen_attr(WHITE_ON_BLACK);
  clear_screen();
  print("> ");

  isr_install();
  irq_install();

  init_timer(50);

  // pid = 0;

  // process_table = (void*) malloc(sizeof(struct pcb*) * MAX_PROCESSES);

  // int i = 0;
  // for(i = 0; i < MAX_PROCESSES; i++)
  //   process_table[i] = 0x00;  // init each value to null value

  // create_process("process 1", process1);
  // create_process("process 2", process2);





  // we allocate 256 * pcb pointer size (~1024 bytes) towards process table

  // char str[32];
  // itoa((int)process_table, str);
  // print("process_table malloc'd at mem addr: ");
  // print(str);
  // print("\n");


  // itoa(sizeof(struct pcb*) * MAX_PROCESSES, str);
  // print("sz malloc'd: ");
  // print(str);
  // print("\n");

  // set up process


  // itoa((int)p, str);
  // print("pcb malloc'd at mem addr: ");
  // print(str);
  // print("\n");

  // p->name = "Process";
  // p->function = proc;

  // process_table[0] = p;

  // print(process_table[0]->name);
  // print("\n");
  // process_table[0]->function();

  // draw_process_table();
}

void draw_process_table()
{
  clear_screen();
  int i;
  for(i = 0; i < 256; i++) {
    if(process_table[i] == 0x00) continue;

    print("\n");

    struct pcb *process = process_table[i];

    char istr[4];
    itoa(i, istr);
    print(istr);
    print(":  ");

    print(process->name);
    print("   ");

    int f_ret = process->function();
    char fstr[10];
    itoa(f_ret, fstr);
    print(fstr);
  }
}

// returns the new process ID
int create_process(char* name, int(*function)(void)) {
  // FIXME: disgustingly dirty pid collision check. if a program exists at the pid,
  // we can just assume we've wrapped around.
  if(process_table[pid] != 0x00)
    panic("process table collision");

  struct pcb *process = (void*) malloc(sizeof(*process));

  process->name = name;
  process->function = function;

  process_table[pid++] = process;

  return pid;
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

