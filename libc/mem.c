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
