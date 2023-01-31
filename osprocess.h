#ifndef _OS_PROCESS_H
#define _OS_PROCESS_H

struct PCB
{
  char *name;
  int pid;                // process ID
  int ppid;               // parent process ID
  int uid;                // user ID of owner
  unsigned char priority; // process scheduler priority
  unsigned char status;   // process status
};

struct process
{
  struct PCB *pcb;
  int *pageTable;
  int *pageDirectory;
  int *stack;
};

#endif
