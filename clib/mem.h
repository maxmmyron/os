#ifndef _UTIL_MEM_H
#define _UTIL_MEM_H

#include "../cpu/types.h"

void mcpy(char* source, char* dest, int nbytes);
void mset(u8 *dest, u8 val, u32 len);

#endif
