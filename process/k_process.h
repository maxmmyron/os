#ifndef _K_PROCESS_H
#define _K_PROCESS_H

struct pcb
{
  char *name;            // process name
  int pid;               // process ID
  int priority;          // process priority
  int status;            // process status
  int (*function)(void); // process function
};

int process1();
int process2();

#endif
