#ifndef _TYPES_H
#define _TYPES_H

// define custom types with no semantic meaning so we have a better grasp on
// allocating non-character bytes.
typedef unsigned int    u32;
typedef int             s32;

typedef unsigned short  u16;
typedef short           s16;

typedef unsigned char   u8;
typedef char            s8;

// create some useful helper functions that allow us to get the lower/higher
// bytes from a 32-bit
#define low_16(address) (u16)((address) & 0xffff)
#define high_16(address) (u16)(((address) >> 16) & 0xffff)

#endif
