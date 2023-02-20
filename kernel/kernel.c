#include "kernel.h"
#include "../libc/str.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../libc/mem.h"
#include "../libc/function.h"
#include "../cpu/paging.h"

// TODO: fix process function location?
int p1v = 0;
int process1() {
  return ++p1v;
}

int p2v = 0;
int process2() {
  p2v += 2;
  return p2v;
}

void kernel_main() {
  set_screen_attr(WHITE_ON_BLACK);
  clear_screen();
  print("> ");

  isr_install();
  irq_install();
  // initalize_paging();

  pid = 0;

  process_table = (void*) malloc(sizeof(struct pcb*) * MAX_PROCESSES);

  int i = 0;
  for(i = 0; i < MAX_PROCESSES; i++)
    process_table[i] = 0x00;  // init each value to null value

  create_process("process 1", process1);
  create_process("process 2", process2);

  add_timer_callback(draw_process_table);
  add_timer_callback(pause_process);
}

void draw_process_table(unsigned int tick)
{
  clear_screen();
  int i;

  char tick_str[32];
  itoa(tick, tick_str);
  print(tick_str);
  print("\n");
  for(i = 0; i < MAX_PROCESSES; i++) {
    if(process_table[i] == 0x00) continue;

    print("\n");

    struct pcb *process = process_table[i];

    char istr[4];
    itoa(i, istr);
    print(istr);
    print(":  ");


    print(process->name);
    print("   ");

    char sstr[3];
    itoa(process->status, sstr);
    print(sstr);
    print("   ");

    // guard clause in case process status isn't run state
    if(process->status != 0) continue;

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

  struct pcb *process = (void*) malloc(sizeof(*process));

  process->name = name;
  process->function = function;

  int i;
  int wrapped = 1;
  for(i = 0; i < MAX_PROCESSES; i++) {
    if(process_table[i] != 0x00) continue;

    process_table[i] = process;
    wrapped = 0;
    break;
  }

  if(wrapped) {
    panic("process table full");
  }

  return pid;
}

void pause_process(unsigned int tick) {
  // pause after 5 seconds
  if(tick > 250) {
    process_table[0]->status = 1;
  }
  // unpause after another 5
  if(tick > 500) {
    process_table[0]->status = 0;
  }

  if(tick > 400) {
    remove_process(1);
  }
}

void remove_process(int pid) {
  if(process_table[pid] == 0x00) return;

  process_table[pid] = 0x00;

  // TODO: FIXME: add free() func (after paging)
  // free(process);
}

void user_input(char* input) {
  if(strcmp(input, "END") == 0) {
    print("shutdown\n");
    asm volatile("hlt");
  }

  if(strcmp(input, "TPANIC") == 0) {
    asm volatile("int $3");
  }

  print("command: ");
  print(input);
  print("\n> ");
}

