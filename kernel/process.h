#ifndef _PROCESS_H
#define _PROCESS_H

#include "../cpu/type.h"

struct pcb
{
  char *name;             // process name       8
  u8 proc_id;             // process ID         1
  u8 parent_id;
  u8 priority;            // process priority   1
  u8 status;              // process status     1
                          //  0:  ready
                          //  1:  running
                          //  2:  blocked
                          //  3:  terminated (i.e. queued for termination)

  int (*function)(void);  // process function   4
};

#endif
