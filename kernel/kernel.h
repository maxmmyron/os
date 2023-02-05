#ifndef _KERNEL_H
#define _KERNEL_H

#define MAX_PROCESSES 256

struct pcb **process_table;

unsigned char pid = 0;

void k_main();

int create(char *name, int priority, void *function);

int terminate(int pid);

int idle();

int schedule_processes();

#endif
