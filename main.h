// #ifndef _MAIN_H
// #define _MAIN_H

// // The maximum number of processes that can exist at any once time.
// #define MAX_PROCESSES 256

// // The process table, defined as an array of pointers to process control blocks.
// struct pcb **processTable;

// unsigned char pid = 0;

// int main(int argc, char *argv[]);

// /**
//  * Creates a new process and adds it to the process table.
//  *
//  * Returns the process ID of the new process.
//  */
// int create(char *name, int priority, void *function);

// /**
//  * Frees the memory allocated to the process and removes it from the process list.
//  *
//  * Returns the process ID of the destroyed process (i.e., the new free slot)
//  */
// int terminate(int pid);

// int idle();

// int scheduleTS();

// #endif
