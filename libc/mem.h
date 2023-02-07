#ifndef _UTIL_MEM_H
#define _UTIL_MEM_H

#include "../cpu/types.h"

void mcpy(u8 *source, u8 *dest, int nbytes);
void mset(u8 *dest, u8 val, u32 len);
u32 malloc(u32 size, int align, u32 *physical_addr);

#endif
