#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "osprocess.h"

int main(int argc, char *argv[])
{
  processTable = malloc(sizeof(struct process *) * MAX_PROCESSES);
  for (int i = 0; i < MAX_PROCESSES; i++)
    processTable[i] = NULL;

  pid = create("idle", 0, 0, 0);

  for (int i = 1; i < MAX_PROCESSES; i++)
  {
    char *name = malloc(10);
    sprintf(name, "process %d", pid);
    pid = create(name, 0, 0, 0);
  }

  for (int i = 1; i < MAX_PROCESSES; i++)
    pid = terminate(i);

  free(processTable);

  return 0;
}

int create(char *name, int ppid, int uid, unsigned char priority)
{
  if (processTable[ppid] == NULL)
    return -1;

  while (processTable[pid] != NULL)
    pid++;

  struct process *p = malloc(sizeof(*p));

  p->pcb->name = name;
  p->pcb->pid = pid;
  p->pcb->ppid = ppid;
  p->pcb->uid = uid;
  p->pcb->priority = priority;
  p->pcb->status = 0;

  processTable[pid] = p;

  return pid;
}

int terminate(int pid)
{
  struct process *p = processTable[pid];

  free(p);
  p = NULL;
  processTable[pid] = NULL;

  return pid;
}
