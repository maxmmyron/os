#ifndef _KERNEL_H
#define _KERNEL_H

#include "process.h"
#include "../cpu/type.h"

#define MAX_PROCESSES 256

u32 malloc_addr; // use a base address from which we can align our malloc calls

struct pcb **process_table; // The process table. we define this as a pointer
                            // to a dynamically allocated array of pcb pointers

void kernel_main();
void user_input(char *input);
void kernel_panic(char *exception_message);

#endif
