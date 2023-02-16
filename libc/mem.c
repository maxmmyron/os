#include "mem.h"

// a simpler implementation of memcpy, renamed such that we don't encounter
// possible namespace collisions
void mcpy(u32 *source, u32 *dest, int nbytes) {
  int i;
  for (i = 0; i < nbytes; i++)
    *(dest + i) = *(source + i);
}

void mcpy8(u8 *source, u8 *dest, int nbytes) {
  int i;
  for (i = 0; i < nbytes; i++)
    *(dest + i) = *(source + i);
}

void mset(u32 *dest, u32 val, u32 len) {
    u32 *temp = (u32 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

u32 placement_address = 0x10000;

u32 malloc_internal(u32 size, int align, u32 *phys_addr) {
  if (align == 1 && (placement_address & 0x00000FFF)) // If the address is not already page-aligned
  {
    // Align placement address
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  if (phys_addr)
  {
    *phys_addr = placement_address;
  }
  u32 tmp = placement_address;
  placement_address += size;
  return tmp;
}
