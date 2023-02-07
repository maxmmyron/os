#include "mem.h"

// internal function definitions
void *malloc_internal(u32 size, int align, u32 *phys_addr);

// a simpler implementation of memcpy, renamed such that we don't encounter
// possible namespace collisions
void mcpy(u8 *source, u8 *dest, int nbytes) {
  int i;
  for (i = 0; i < nbytes; i++)
    *(dest + i) = *(source + i);
}

void mset(void *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

u32 placement_address = 0x10000;

void *malloc(u32 size) {
  void *ptr = malloc_internal(size, 0, 0);
  return ptr;
}

void *malloc_a(u32 size) {
  void *ptr = malloc_internal(size, 1, 0);
  return ptr;
}

void *malloc_p(u32 size, u32 *phys_addr) {
  void *ptr = malloc_internal(size, 0, phys_addr);
  return ptr;
}

void *malloc_ap(u32 size, u32 *phys_addr){
  void *ptr = malloc_internal(size, 1, phys_addr);
  return ptr;
}

void *malloc_internal(u32 size, int align, u32 *phys_addr) {
  if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  if (phys_addr)
  {
    *phys_addr = placement_address;
  }
  u32 tmp = placement_address;
  placement_address += size;
  return (void*) tmp;
}
