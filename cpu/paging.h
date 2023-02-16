#ifndef _PAGING_H
#define _PAGING_H

#include "type.h"
#include "isr.h"

// in this header file we set up the necessary structs and methods to implement
// memory paging: a memeory management scheme which allows us a few benefits:
//  1. larger virtual memory size:  we don't need the full physical memory to be
//                                  present for any one process. this gives us
//                                  the added benefit of being able to use much
//                                  larger amounts of virtual memory than the
//                                  physical amount of memory installed on our
//                                  machine.
//  2. page-level protection:       user processes can only see and modify their
//                                  own address space, which proces a level of
//                                  hardware based isolation.

// paging is achieved through the two of two tables, each 1024 4-byte entires in
// size:
// Paging directory:  a table in memory that the MMU uses to find each page
//                    table. each entry points to a page table
// page table:        a table in memory that describes how the MMU should
//                    translate a range of addresses. each entry points to a
//                    4KiB physical page frame.
// the paging directory and page table represents a linear 4-GiB virtual memory
// map which can be used to virtually represnt 4-GiB of memory

// a 32-bit page structure. composed of 5 flag bits, 7 unused "padding bits",
// and a 20-bit frame address;
typedef struct {
  u32 present   : 1;  // whether or not the page is physically present in
                      // memory.
                      // if clear:  the page does not physically exist in
                      //            memory. occurs in instances where page is
                      //            swapped out. OS should handle this as a page
                      //            fault would occur if a page is called and
                      //            not present
                      // if set:    the page is physically present in memory.

  u32 rw        : 1;  // controls read/write capaiblities of page.
                      // if clear:  page is read-only
                      // if set:    page is read/write
  u32 user      : 1;  // controls access to page based on privlege level.
                      // if clear:  only supervisor can access page
                      // if set:    all can access page
  u32 accessed  : 1;  // used to know whether the page entry was read during
                      // virtual access.
                      // if set: the page was read during virtual access
                      // translation
  u32 dirty     : 1;  // used to know whether the page has been written to.
  u32 unused    : 7;  // pack together unused bits (since frame should be right-
                      // shifted 12 bits)
  u32 frame     : 20; // frame address (right-shifted 12 bits). a frame address
                      // points to a 4KiB address of memory

} __attribute__((packed)) page_table_entry;

// the page table contains 1024 page table entires
typedef struct {
  page_table_entry pages[1024];
} __attribute__((packed)) page_table;

// the page directory consits of 1024 page tables
typedef struct {
  page_table *page_tables[1024];   // create an array of pointers to page tables.

  u32 tables_phys[1024];        // array of pointers to the *physical* addresses
                                // of each page table. we use this for loading
                                // the page table locations into the cr3
                                // register

  void* physical_addr;          // the physical address of tables_phys. we need
                                // this because when we allocate our kernel heap
                                // the directory may be in a different location
                                // in virtual memory.
} __attribute__((packed)) page_directory;

// sets up the environment with page directories, and enabled paging at the asm
// level.
void initalize_paging();

// loads the specified page directory into the CR3 register.
void switch_page_directory(page_directory *dir);

// returns a pointer to the page required.
// if make is set, then we will create the page table if it doesn't exist where
// it should
page_table_entry *get_page(u32 addr, int make, page_directory *dir);

// handler function in the event of page faults.
void handle_page_fault(registers_t regs);

#endif
