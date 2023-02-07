#ifndef _UTIL_MEM_H
#define _UTIL_MEM_H

#include "../cpu/type.h"

void mcpy(u8 *source, u8 *dest, int nbytes);
void mset(void *dest, u8 val, u32 len);

// vanilla malloc functions. return a virtual address pointer
void *malloc(u32 size);
void *malloc_a(u32 size);                   // page-aligned
// malloc functions that return a physical address pointer
void *malloc_p(u32 size, u32 *phys_addr);
void *malloc_ap(u32 size, u32 *phys_addr);  // page-aligned

#endif
