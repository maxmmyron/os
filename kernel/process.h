#ifndef _PROCESS_H
#define _PROCESS_H

#include "../cpu/type.h"

struct pcb
{
  char *name;             // process name       8
  u8 proc_id;             // process ID         1
  u8 priority;            // process priority   1
  u8 status;              // process status     1
                          //  0:  running
                          //  1:  paused
  int (*function)(void);  // process function   4 ?
};

#endif
