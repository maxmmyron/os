# Lab 1 Report

Max Myron

CS 2506

---

## 1. Introduction
The purpose of this lab is to analyze and implement a prototypical example of a process, its life cycle, and scheduling strategies one might find in an operating system.

In this report, I plan to demonstrate an example of a general-purpose operating system, which schedules processes using the round-robin algorithm. I chose a general-purpose operating system to remove the concern of processes not executing in a single timeslice. Instead, processes may take as long as they need to execute; execution time is split up and managed by the scheduler algorithm.

The operating system itself is similar to a Linux kernel; it is extremely bare-bones and handles simple process switching.

The provided code in this report is written in standard C. This scheduler implementation would work in an x86-based kernel. Assembly instructions are provided in standard x86 assembly language.

---

## 2. Process Entity
### Process Struct
A process is the instance of a program running on a computer or the context associated with a program in execution. Processes perform a variety of functions, from general-purpose user processes to lower-level OS processes.

In a general-purpose operating system, the process entity must have enough information stored such that it can resume running from its original state in cases where it does not execute in one time slice.

A minimal process context for a general-purpose operating system includes the following:

- **Name**: the name of the process.
- **PID**: The ID of the process itself.
- **PPID**: the ID of the parent process (i.e. the process that this process is forked from)
- **Status**: The status of the process:
  - (`0b00`) *New*: The process has been created, but has not yet been added to the ready queue (i.e., is in the waiting queue.)
  - (`0b01`) *Ready*: The process is not executing, but is ready to.
  - (`0b10`) *Running*: The process is currently executing
  - (`0b11`) *Terminated*: The process has completed execution and will be removed from the processes table.
- **Callback**: The callback function to run. In this minimal demonstration, this serves as the program's main function.

I intentionally omitted the UID in the process context to keep a more minimal environment. All processes are assumed to be owned by the kernel.

The process context struct can be written out as such in C:
```c
// process.h

// defines a new PCB struct, which is used to define PCBs in code.
struct pcb
{
  char    *name;      // process name       8 bytes
  uint8_t pid;        // process ID         1 byte
  uint8_t ppid;       // parent process ID  1 byte
  uint8_t status: 2;  // process status     2 bits
                      //  0:  new
                      //  1:  ready
                      //  2:  running
                      //  3:  terminated (i.e. queued for termination)

  int (*callback)(void);  // process callback   4 bytes
};
```

The process context contains a name for the program (primarily to tell processes apart from one another), a PID, a Parent PID, a 2-bit process status field, and a void pointer to a callback function. The callback function is an analog to the process stack, data section, and text section, and works much the same when running this simplified model in C.

The lack of a priority field is intentional. The usage of a round-robin algorithm for scheduling process execution eliminates the need for priority, as all processes receive fairer treatment in terms of CPU time allocation.

### Process Allocation

I decided to use a single byte (as an unsigned char) as the integer range within which processes are allocated free PIDs. I chose a smaller range to take advantage of the cyclic nature of unsigned char overflow.

The below example shows the process by which a process is enqueued into either the process ready queue or process wait queue. `enqueue_process` makes use of a helper function `get_free_pid`, which retrieves the next available PID after the last occupied PID.

If there is no available PID, then the process is placed into a waiting queue, which will be dealt with the next time there is a free PID available. Processes in waiting take priority over processes attempting to be added through `enqueue_process`.

```c
// kernel.c

// returns the next free PID from the last_pid start point if one exists.
// if one does not exist (at least, one that isn't eq. to last_pid) then
// return last_pid. we need to perform an extra check.
uint8_t get_free_pid(uint8_t last_pid) {
  uint8_t curr_pid = last_pid+1;
  while(curr_pid != last_pid) {
    if(processes[curr_pid]) curr_pid++;
    else return curr_pid;
  }
  return last_pid;
}

// enqueues a new process into the ready queue given the last free pid.
// If no free_pid exists, add the process to a waiting queue and return
// the original PID passed in.
uint8_t enqueue_process(struct pcb* process, uint8_t last_pid) {
  if(process->pid) return get_free_pid(process->pid);

  uint8_t free_pid = get_free_pid(last_pid);
  // if free_pid equals last_pid AND that PID is taken in processes table,
  // then add process to allocation queue
  if(free_pid == last_pid && processes[free_pid]) {
    enqueue(alloc_queue, process);
  }

  // assign free_pid to process and reset status (just in case)
  process->pid = free_pid
  process->status = 0;

  // assign process pointer to free_pid index of processes arr, and
  // enqueue into the ready queue
  processes[free_pid] = process;
  enqueue(ready_queue, process);

  return get_free_pid(free_pid);
}
```

### Process Storage

The struct to store all allocated processes in memory is an array with a length of 256. An array is one of the simplest data structures for storing large amounts of the same data structure, and I decided to lump in array ID validation into the rest of my PID allocation code which removes the worries of overwriting processes for a certain PID.

There are two other structs used in the operating system during a process lifetime. If a process is unable to be added, it is instead added to a waiting queue that is checked periodically. If a PID is available, and no processes are to be added, then the kernel will begin adding process from the waiting queue to the process table. Another queue exists when a process is ready for execution. All process are initially added to this queue upon successful PID allocation, but can also be added when they move back from the `blocked` or `running` state to the `ready` state.

```c
// kernel.h

// define the max number of processes that can be held at any one time.
#define MAX_PROCESSES 256

// The process table. we define this as a pointer to a dynamically
// allocated array of pcb pointers
struct pcb **processes;
// define two queues which handle processes that need to be allocated and
// processes that are ready for exec. process_queue contains an array
// that is MAX_PROCESSES in length, as well as pointers to the front and
// rear of the queue.
struct process_queue alloc_queue;
struct process_queue ready_queue;


// kernel.c

// dynamically allocate the memory required for the process table.
// We want enough memory for 256 processes, so we multiply the size of
// the process control block struct by the number of processes allowed.
process_table = malloc(sizeof(struct pcb*) * MAX_PROCESSES);
```

---

## 3. Process Life Cycle

A process can be in one of several states at any given time, since a process may be queued to execute, executing, or ready to terminate and be freed from memory. A scheduler implementation must be able to implement a system that can recognize what state a process should be in and make it so.

### Process States

There are four states that a process can have in this model:

| State | Associated Events | Description |
| --- | --- | --- |
| *New* | `enqueue_process()` | The process has just been created. The process is assumed to have a state of `ready`, and will be added to either the ready queue via `admit_process` or to the wait queue |
| *Ready* | `admit_process()` | The process is ready to be executed. A process is added to the ready queue when it is first enqueued upon process creation, or when an I/O or event block has been completed and the process is no longer blocked from executing. |
| *Running* | `dispatch_process()` | The process is currently running. The process can only reach this state from the ready state, as any process must first be in the ready queue to be chosen for execution. |
| *Terminated* | `release_process()` | The process has finished executing and can be released from memory. In `release_process()`, the process will be removed from any queues it may be in, as well as from the overall process array. its PID will be freed. |

Note the lack of a `blocked` state. This is due to the nature of the round-robin scheduler, which by design places a limit on the contiguous period a process may have on the CPU. In the event of time slice expiration, I/O, and interrupts, a process is simply placed back into the run queue (as opposed to being blocked for a period of time.)

### Process State Changes

Event functions trigger a change in a process' state.

- `running` -> `ready`: A process moves from the `running` state to the `ready` state in two cases. In the first case, the process has naturally finished its execution within the slot given, however it has not *explicitly* terminated itself using `release_process()`. In this case, it will be moved back to the ready state to perform more operations. In the second case, a process is interupted by the programmable interval timer; the process has its state saved into memory and is queued back into the ready queue.
- `new` -> `ready`: A process moves from the `new` state to the `ready` state when there is an available PID to slot it into.
- `running` -> `terminated`: This state change occurs when a process has completed its execution and will not run again. The process will call `release_process()`, which sets its state to `terminated` and sets a cleanup flag that notifies the OS that processes need to be removed from the process array and have their PIDs freed.

### Idle process algorithm

In the event the process queue is empty of processes, the idle process will run. The idle process can be used to save power when the CPU would otherwise be wasting power doing nothing.

The idle process is implemented as a special process, separate from the normal process array. It can be assumed that the idle process is always ready; the process never terminates until the entire system is ready to shut down. The scheduler will run the idle process in the case where the process array is otherwise empty.

```c
// kernel.h

// kernel idle process
void k_idle() {
  asm("hlt");
}
```

The use of the `hlt` instruction is specific; it is the bare minimum that the CPU could possibly do. This instruction serves two purposes:

1. Keep the CPU doing the absolute minimum something without resorting to spinning. `hlt` specifically halts the CPU until an interrupt is received, so a spinning pattern is not necessary
2. Allow the CPU to set itself into a low-power state. `hlt` lets the CPU move into a low-power mode which reduces power usage and heat output, which makes it doubly preferable to spinning.

---

## 4. Scheduler

The scheduler in this model OS follows the *round-robin algorithm*. It is a preemptive scheduling algorithm capable of allocating time slices to processes of arbitrary execution time. The round-robin approach is relatively simple to implement, starvation-free, and eliminates instances of priority inversion.

The scheduler takes in a queue of ready processes (in the form of PCBs), calculates the best job to run for that time slice, and returns a new queue (post time slice runtime). We don’t know the overall runtime of any one process, so the scheduler will keep track of process runtimes and use that to calculate which process should execute.

The round-robin function accepts a queue of processes, which in the pseudo-code above contains a function to exec (the actual process) and a runtime counter. The function initializes a time slice duration and runtime counter.

The implementation of this algorithm explains the lack of need for priority and blocking states within the process context. Because all processes are guaranteed fair treatment in terms of CPU time allocation, there is no need for priority. Because all ready processes are held on a queue, there is no need for a blocked state; processes removed from the running state due to IO/event interrupts are simply queued up to run again.

The scheduler algorithm is a simple function that uses the global `ready_queue` struct, as well as a global `curr_process` process struct:

```c
// kernel.h
#include <time.c>

// define a duration for the time slice (the value here is arbitrary)
#define TIMESLICE 100.0

// array/queues
struct pcb **processes;
struct process_queue ready_queue;

// define a pointer to the current process executing
struct pcb *curr_process;

// kernel.c

curr_process = 0x00;

// handles round-robin scheduling
void schedule_rr() {
  // initialize a counter for the current runtime racked up by processes.
	double runtime_count = 0;

  // if no processes, run kernel idle process
  if(ready_queue.length == 0) k_idle();

	while(runtime_count < TIMESLICE) {
    // dequeue the front process pointer and assign it to curr_process
    curr_process = dequeue(ready_queue);
    curr_process->status = 2;

    // call the exec_process wrapper function.
    // when it returns, add the runtime to the counter
    runtime_count += exec_process();
	}

  // loop back into the scheduler
  schedule_rr();
}

// wrapper fn
double exec_process() {
  clock_t start_time = clock();

  curr_process->callback();

  // check if the process wants to terminate (status == 3)
  if(curr_process->status != 3) {
    curr_process->status = 1;
    enqueue(ready_queue, curr_process);
  } else release_process(curr_process); // free the memory and release the PID.

  clock_t end_time = clock();

  // CLOCKS_PER_SEC defined in <time.h> (i.e. standard C)
  double runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;

  return runtime;
}

// this executes at the end of every time slice and interrupts the currently
// running process.
void handle_timer_interrupt() {
  // add process back into the ready queue
  curr_process->status = 1;
  enqueue(ready_queue, curr_process);

  // restart the scheduler cycle, since we've reached the end of the
  // last cycle.
  schedule_rr();
}
```

There are three primary functions in the above example:

1. `schedule_rr()`: The primary scheduler function. Runs as a loop, which will run processes while able to. If no processes are available, then it will run the idle process by default.
2. `exec_process()`: A wrapper function for executing a process callback function. If a process finishes in this function, it is either requeued into the ready queue or released, depending on the state of the process status at the end of the callback function.
3. `handle_timer_interrupt()`: an interrupt function that is called when the timer interrupt is met. At a high level, this function is called at the end of every timeslice, and fully interrupts whatever the CPU is executing (i.e. the current process.)

The scheduler works in the following steps:

1. run the idle process if no other processes are available
2. **while** the `runtime count < time slice`:
    1. dequeue front-most processes while able to during the time slice.
    2. execute the process
      - **if** the process *finishes* within the time slice:
        1. either add to ready queue or release depending on status
        2. return process runtime and add to runtime count in scheduler

      - **if** the process *does not finish* within the time slice:
        1. queue the process in the ready queue
        2. restart the scheduler (a new time slice has started)

3. loop the scheduler (a new time slice has started)


## 5. Conclusions

This lab was a helpful introduction to how schedulers and processes are stored and executed in an operating system. I was able to use this opportunity to learn more about low-level, x86-specific events that occur when it comes to processes. For example, I was able to learn more about the Programmable Interval Timer, which is crucial in halting the executing of a process on the CPU and relinquishing the CPU back to the scheduler if a process doesn't finish in time.

I did encounter a few challenges while implementing this scheduler, which primarily came due to a lack of experience with x86 hardware, as well as unfamiliarity with process contexts:

1. **time slice expiration**: It was difficult to figure out how to implement a round-robin scheduler such that a process function would be halted once a time slice was used up. I found that timer interrupts generated by the PIT hardware on the CPU were the defacto way of interrupting this execution
2. **ready queue**: I initially tried implementing a ready *array* in order to hold the processes that were ready for execution. I found array index management a bit cumbersome; I instead decided that the small amount of extra overhead for a proper FIFO queue struct would be worth it in the long run.
3. **round-robin algorithm**: the algorithm, while extremely simple by itself, took a bit of thinking to implement in C.

I enjoyed putting together this lab. It was a great excuse to dive deep into C, process handling in Linux-based operating systems, hardware interrupts, and how assembly instructions interop with C and the CPU.
