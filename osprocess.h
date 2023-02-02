#ifndef _OS_PROCESS_H
#define _OS_PROCESS_H

struct PCB
{
  char *name;  // process name
  int pid;                          // process ID
  int priority;                     // process priority
  int status;                       // process status

};

struct process
{
  struct PCB *pcb;
  int *pageTable;
  int *pageDirectory;
  int *stack;
};

#endif
