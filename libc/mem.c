#include "mem.h"

// a simpler implementation of memcpy, renamed such that we don't encounter
// possible namespace collisions
void mcpy(u8 *source, u8 *dest, int nbytes) {
  int i;
  for (i = 0; i < nbytes; i++)
    *(dest + i) = *(source + i);
}

void mset(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

u32 free_mem_addr = 0x10000;

void *malloc_aligned(u32 size, int align) {
  // if pages are aligned to 4k (0x1000)
  if(align == 1 && (free_mem_addr & 0xfffff000)) {
    free_mem_addr &= 0xfffff000;
    free_mem_addr += 0x1000;
  }

  void *ptr = (void*)free_mem_addr;
  free_mem_addr += size;  // increment pointer to next free space based on size
                          // of malloc'd memory
  return ptr;
}
