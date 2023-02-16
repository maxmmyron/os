#include "paging.h"
#include "isr.h"
#include "type.h"
#include "../libc/function.h"
#include "../libc/str.h"
#include "../libc/mem.h"

// The kernel's page directory
page_directory *kernel_directory=0;

// The current page directory;
page_directory *current_directory=0;

// create a bitset of frames
u32 *frames;
u32 nframes;

extern u32 placement_address;

// bitset algorithm macros
#define BIT_INDEX(a) (a/(8*4))
#define BIT_OFFSET(a) (a%(8*4))

// sets a bit in the frames bitset
static void set_frame(u32 frame_addr) {
  u32 frame = frame_addr / 0x1000;
  u32 idx = BIT_INDEX(frame);
  u32 offset = BIT_OFFSET(frame);

  frames[idx] |= (0x1 << offset);
}

// clears the bit from the frames bitset
static void clear_frame(u32 frame_addr) {
  u32 frame = frame_addr / 0x1000;
  u32 idx = BIT_INDEX(frame);
  u32 offset = BIT_OFFSET(frame);

  frames[idx] &= ~(0x1 << offset);
}

// tests whether or not a bit is set
// static u32 test_frame_set(u32 frame_addr) {
//   u32 frame = frame_addr / 0x1000;
//   u32 idx = BIT_INDEX(frame);
//   u32 offset = BIT_INDEX(frame);

//   return (frames[idx] & (0x1 << offset));
// }

// finds the first free frame
static u32 get_first_frame() {
  u32 i, j;
  for (i=0; i < BIT_INDEX(nframes); i++) {
    // if all bits are set, then we can exit early.
    if (frames[i] == 0xFFFFFFFF) return (u32)-1;
    for (j = 0; j < 32; j++) {
      u32 bit_test = 0x1 << j;
      if(!(frames[i] & bit_test)) {
        return i*4*8+j;
      }
    }
  }
  // idk, if we somehow reach this just assume all frames set?
  return (u32)-1;
}

void allocate_frame(page_table_entry *page, int is_kernel, int is_writeable) {
  // if frame is already allocated, then we can return early.
  if(page->frame != 0) return;

  // get the index of the first available frame
  u32 idx = get_first_frame();
  if(idx == (u32)-1) panic("no free frames");

  set_frame(idx*0x1000); // set a new frame at the index;

  // set page bits and address
  page->present = 1;
  page->rw=(is_writeable) ? 1 : 0;
  page->user=(is_kernel) ? 0 : 1;
  page->frame=idx;
}

void free_frame(page_table_entry *page) {
  u32 frame;

  // if the page wasn't actually allocated return early
  if(!(frame = page->frame)) return;

  clear_frame(frame);
  page->frame = 0x0;  // remove frame
}

// --------------------------
// public paging API
// --------------------------

void initalize_paging(){
  // for now, assume the physical memory space is 16 MB.
  u32 mem_end_page = 0x1000000;

  nframes = mem_end_page / 0x1000;
  frames = (u32*) malloc(BIT_INDEX(nframes));
  mset(frames, 0, BIT_INDEX(nframes));

  // create a new page directory
  kernel_directory =  (page_directory*)malloc_a(sizeof(page_directory));
  mset((u32*)kernel_directory, 0, sizeof(page_directory));
  // set the current page directory to the kernel page directory
  current_directory = kernel_directory;

  // map physical address -> virtual address from 0x0 to the end of the used
  // memory space so we can access it as if paging isn't enabled.
  u32 i = 0;
  while (i < placement_address) {
    // allocate new frames that are read-only from userspace
    allocate_frame(get_page(i, 1, kernel_directory), 0, 0);
    i += 0x1000;
  }

  // before we enable paging we need to register our page fault handler to
  // isr 14
  register_interrupt_handler(14, handle_page_fault);

  // enable paging
  switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory *dir) {
  current_directory = dir;

  // char dir_str[40];
  // htoa((int)(&dir->tables_phys), dir_str);
  // panic(dir_str);

  // move the physical address of the page table into the cr3 register
  asm volatile("mov %0, %%cr3":: "r"(&dir->tables_phys));
  u32 cr0;
  // move the contents of the cr0 register into cr0 variable (since we can't
  // directly edit the contents of cr0)
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // enable the paging bit
  // move cr0 variable back into cr0 register
  asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_table_entry *get_page(u32 addr, int make, page_directory *dir) {
  // divide by 0x1000 to get as index
  addr /= 0x1000;
  // get page table with this addr index
  u32 table_idx = addr / 1024;
  if(dir->page_tables[table_idx])
    // if this table is already assigned, then return the page at the addr
    return &dir->page_tables[table_idx]->pages[addr%1024];
  else if(make) {
    // otherwise, make a new page
    u32 phys_table_addr;
    // use malloc_ap so we can store the physical address
    dir->page_tables[table_idx] = (page_table*)malloc_ap(sizeof(page_table), &phys_table_addr);
    mset((u32*)(dir->page_tables[table_idx]), 0, 0x1000);
    dir->tables_phys[table_idx] = phys_table_addr | 0x7;
    return &dir->page_tables[table_idx]->pages[addr%1024];
  }
  else return 0;
}

void handle_page_fault(registers_t regs) {
  asm("mov %0, %%eax":: "r"(regs.err_code));
  while(1);
  //u32 fault_addr;
  // move the fault address into the cr2 register
  //asm volatile("mov %%cr2, %0" : "=r"(fault_addr));

  // sus out error cause from error code
  // int present = !(regs.err_code & 0x1);   // Page not present
  // int rw = regs.err_code & 0x2;           // attempt write operating on
  //                                         // read-only page
  // int us = regs.err_code & 0x4;           // attempt to read kernel page in user
  //                                         // mode
  // int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of
  //                                         // page entry?
  // int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

  //   // Output an error message.
  // char page_fault_msg[256];

  // page_fault_msg = "Page fault: (";
  //  if (present) page_fault_msg += "present ";
  //  if (rw) page_fault_msg += "read-only ";
  //  if (us) page_fault_msg += "user mode ";
  //  if (reserved) page_fault_msg += "reserved ";
  //  page_fault_msg += ") at 0x";

  // char fault_addr_str[32];
  // htoa(fault_addr, fault_addr_str);

  // page_fault_msg+=fault_addr_str;

  //panic("page fault");
}
