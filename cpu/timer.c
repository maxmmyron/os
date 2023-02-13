// the Programmable Interval Timer (PIT). oscillates at a specific frequency
// which can be used to control a timer at some rate.

// hardware:  Intel 8253
//            runs at 105/88 = 1.1931818... MHz.

#include "timer.h"
#include "isr.h"
#include "ports.h"
#include "../libc/function.h"

// the system hardware that controls the timer is the Intel 8253, which runs at
// 1.1931818...Mhz. Nowadays this is primary a holdover from PC with an IBM
// Color Graphcis Adapters (CGA).
#define PIT_CLOCK_RATE = 1193180

// create a new array containg void ptrs to functions with an unsigned int param
// the unsigned int param is the current tick, passed from timer.c into the func
typedef void (*callback)(unsigned int);
callback callbacks[16];

u32 tick = 0;

// keep a running tally of callbacks added
unsigned char callback_count = 0;

static void timer_callback(registers_t regs) {
  tick++;

  int i = 0;
  for(i = 0; i < callback_count; i++) {
    if(callbacks[i] != 0x00) callbacks[i](tick);
  }

  UNUSED(regs);
}

int add_timer_callback(void (*callback)(unsigned int)) {
  if(callback_count == 16) return 0;
  callbacks[callback_count++] = callback;
  return 1;
}

void init_timer(u32 freq) {
  // install the timer function to IRQ0
  register_interrupt_handler(IRQ0, timer_callback);

  // initalize each element to 0 ptr
  // TODO: FIXME: move callbacks from timer.c to dedicated scheduler file
  int i;
  for(i = 0; i < 16; i++) callbacks[i] = 0x00;

  // get the PIT value: hardware clock
  u32 divisor = 1193180 / freq; // fire PIT at desired freq by
                                // dividing oscillator clock by our
                                // target freq.

  u8 low = (u8)(divisor & 0xFF);          // get lower 8 bits of divisor using
  u8 high = (u8)((divisor >> 8) & 0xFF);  // get upper 8 bits of divisor

  // write to the PIT chip

  // 00100100
  // 00110000
  port_byte_out(0x43, 0x36);  // send a command to port 0x40 to notify that we
                              // are writing to PIT.

  // write to channel 0 our
  port_byte_out(0x40, low);   // write low byte to channel 0
  port_byte_out(0x40, high);  // write high byte to channel 0
}
