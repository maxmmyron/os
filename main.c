#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "osprocess.h"

int main(int argc, char *argv[])
{
  printf("hey");
  processTable = malloc(sizeof(struct pcb *) * MAX_PROCESSES);
  for (int i = 0; i < MAX_PROCESSES; i++)
    processTable[i] = NULL;

  pid = create("idle", 0, idle);

  pid = create("p1", 0, process1);
  pid = create("p2", 0, process2);

  for (int i = 0; i < 10; i++)
    scheduleTS();

  // shutdown

  for (int i = 1; i < MAX_PROCESSES; i++)
    pid = terminate(i);
  free(processTable);

  return 0;
}

int create(char *name, int priority, void *function)
{
  while (processTable[pid] != NULL)
    pid++;

  struct pcb *p = malloc(sizeof(*p));

  p->name = name;
  p->priority = priority;
  p->status = 0;
  p->function = function;

  processTable[pid] = p;

  return pid;
}

int terminate(int pid)
{
  struct pcb *p = processTable[pid];

  free(p);
  p = NULL;
  processTable[pid] = NULL;

  return pid;
}

int idle()
{
  printf("idling...\n");
  return 0;
}

int scheduleTS()
{
  int (*function)(void);
  printf("--------------\n");
  for (int i = 0; i < MAX_PROCESSES; i++)
  {
    if (processTable[i] != NULL)
    {
      function = processTable[i]->function;
      function();
    }
  }

  return 0;
}
