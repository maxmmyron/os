#include "ports.h"

// a number of low-level port I/O functions used by our hardware drivers.
// these use inline assembly to inject little snipps of assembly into our
// function bodies.

u8 port_byte_in(u16 port) {
  // A wrapper function that reads a byte from the port
  // "=a" (result)  - put al register into var result when finished
  // "d"  (port)    - load edx with port

  u8 result;
  // GCC adopts a different assembly syntax known as GCC
  // instead of nasm syntax (mov al, 0xff; move 0xff into register al)
  // it's                   (mov 5, al)
  // likewise, the use of the double % is due to % being C's escape character.
  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

void port_byte_out(u16 port, u8 data) {
  // "a" (data) - load eax with data
  // "d" (port) - load edx with port
  __asm__ __volatile__("out %%al, %%dx" : :"a" (data), "d" (port));
}

u16 port_word_in(u16 port) {
  u16 result;
  __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

void port_word_out(u16 port, u16 data) {
  __asm__ __volatile__("out %%ax, %%dx" : :"a" (data), "d" (port));
}


