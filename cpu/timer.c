#include "timer.h"
#include "isr.h"
#include "ports.h"
#include "../libc/function.h"

typedef void (*callback)(void);
callback callbacks[16];

u32 tick = 0;
// running tally of callbacks added
unsigned char callback_count = 0;

static void timer_callback(registers_t regs) {
  tick++;

  int i = 0;
  for(i = 0; i < callback_count; i++) {
    if(callbacks[i] != 0x00) callbacks[i]();
  }

  UNUSED(regs);
}

int add_timer_callback(void (*callback)(void)) {
  if(callback_count == 16) return 0;
  callbacks[callback_count++] = callback;
  return 1;
}

void init_timer(u32 freq) {
  // install the timer function
  register_interrupt_handler(IRQ0, timer_callback);

  // initalize each element to 0 ptr
  int i;
  for(i = 0; i < 16; i++) callbacks[i] = 0x00;

  // get the PIT value: hardware clock
  u32 divisor = 1193180 / freq;
  u8 low = (u8)(divisor & 0xFF);
  u8 high = (u8)((divisor >> 8) & 0xFF);

  port_byte_out(0x43, 0x36); // command port
  port_byte_out(0x40, low);
  port_byte_out(0x40, high);
}
