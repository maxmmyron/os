// the IDT, or interrupt descriptor table, is a similar structure to the GDT
// described in boot/gdt.asm. Interrupts stop whatever the cpu is doing, and
// force it to do something else, like when a key is pressed.

#ifndef _IDT_H
#define _IDT_H

#include "types.h"

// segment selectors
#define KERNEL_CS 0x08

// interrupt gate definition
typedef struct {
  u16 low_offset;   // lower 16 bits of handler function address
  u16 sel;           // the kernel segment selector
  u8 always0;       // always 0.
  u8 flags;         // bit 7:     "Interrupt present"
                    // bits 6-5:  caller privilege (0 = kernel ... 3 = user)
                    // bits 4:    set to 0 for interrupt gates
                    // bits 3-0   set to 1110 -> 14 in decimal -> "32 bit
                    //            interrupt gate"
  u16 high_offset;  // higher 16 bits of handler function address
} __attribute__((packed)) idt_gate_t;

// decribes a pointer to an array of interrupt handlers.
typedef struct {
  u16 limit;
  u32 base;         // the address of the first element in idt
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_register;

void set_idt_gate(int n, u32 handler);
void set_idt();

#endif
