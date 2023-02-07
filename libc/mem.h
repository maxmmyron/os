#ifndef _UTIL_MEM_H
#define _UTIL_MEM_H

#include "../cpu/type.h"

void mcpy(u32 *source, u32 *dest, int nbytes);
void mcpy8(u8 *source, u8 *dest, int nbytes);
void mset(u32 *dest, u32 val, u32 len);

u32 malloc_internal(u32 size, int align, u32 *phys_addr);

#define malloc(size) malloc_internal(size, 0, 0);
#define malloc_a(size) malloc_internal(size, 1, 0);
#define malloc_p(size, phys_addr) malloc_internal(size, 0, phys_addr);
#define malloc_ap(size, phys_addr) malloc_internal(size, 1, phys_addr);

#endif
