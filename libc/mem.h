#ifndef _UTIL_MEM_H
#define _UTIL_MEM_H

#include "../cpu/types.h"

#define malloc(size) malloc_aligned(size, 1);

void mcpy(u8 *source, u8 *dest, int nbytes);
void mset(u8 *dest, u8 val, u32 len);

void *malloc_aligned(u32 size, int align);

#endif
