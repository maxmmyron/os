// a number of low-level port I/O functions used by our hardware drivers.
// these use inline assembly to inject little snipps of assembly into our
// function bodies.

unsigned char port_byte_in(unsigned short port) {
  // A wrapper function that reads a byte from the port
  // "=a" (result)  - put al register into var result when finished
  // "d"  (port)    - load edx with port

  unsigned char result;
  // GCC adopts a different assembly syntax known as GCC
  // instead of nasm syntax (mov al, 0xff; move 0xff into register al)
  // it's                   (mov 5, al)
  // likewise, the use of the double % is due to % being C's escape character.
  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
  // "a" (data) - load eax with data
  // "d" (port) - load edx with port
  __asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port) {
  unsigned short result;
  __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

void port_word_out(unsigned short port, unsigned short data) {
  __asm__("out %%ax, %%dx" : :"a" (data), "d" (port));
}


