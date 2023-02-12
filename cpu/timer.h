#ifndef _TIMER_H
#define _TIMER_H

#include "type.h"

// adds a callback to the callbacks array if there is available space.
// returns 1 if successful
int add_timer_callback(void (*callback)(unsigned int));

void init_timer(u32 freq);

#endif
