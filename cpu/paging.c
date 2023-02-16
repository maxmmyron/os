#include "paging.h"
#include "isr.h"
#include "type.h"
#include "../libc/function.h"
#include "../libc/str.h"
#include "../libc/mem.h"


// set up a global, page-aligned, static page directory.
// IRL we should be dynamically allocating this but for now this is doable
u32 page_directory[1024] __attribute__((aligned(4096)));

// create a new page table (similar 1024-size structure to page directory)
u32 first_page_table[1024] __attribute__((aligned(4096)));

void initalize_paging() {
  u32 i;
  for (i = 0; i < 1024; i++) {
    // blank the page directory by setting each entry to:
    // not present        (bit 1 = 0)
    // write-enabled      (bit 2 = 1)
    // kernel-mode access (bit 3 = 0)
    page_directory[i] = 0x00000002;
  }

  for (i=0; i < 1024; i++) {
    // setup following attributes: supervisor level, read/write, present
    first_page_table[i] = (i * 0x1000) | 3;
  }

  page_directory[0] = ((u32) first_page_table) | 3;

  // enable paging in asm

  load_page_dir(page_directory);
  enable_paging();
}

void handle_page_fault(registers_t regs) {
  asm("mov %0, %%eax":: "r"(regs.err_code));
  while(1);
}
