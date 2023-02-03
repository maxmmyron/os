#ifndef _OS_PROCESS_H
#define _OS_PROCESS_H

struct pcb
{
  char *name;            // process name
  int pid;               // process ID
  int priority;          // process priority
  int status;            // process status
  int (*function)(void); // process function
};

int process1()
{
  printf("process 1\n");
  return 0;
}

int process2()
{
  printf("process 2\n");
  return 0;
}

#endif
