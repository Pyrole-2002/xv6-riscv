# REPORT Assignment - 4
### Authors: Aryan Bansal, Hardik Sharma

This report explains briefly the changes and improvements made to the existing xv6 riscv operating system.

## Running the modified xv7 riscv

- `cd` to the directory containing `Makefile` and run `make qemu SCHEDULER=[OPTIONS] CPUS=[1-8]`.
The `SCHEDULER` and `CPUS` flags are optional. If not specified, SCHEDULER=RR and CPUS=8.

## Specification 1: System Calls

### System Call 1: `trace`

- The `trace` system call, when called upon by a process, tracks down the system calls being issued by that
process during its execution. The children of the calling processes are also tracked, but any other process
executing parallely is not tracked, unless the process itself calls trace.

- The trace system call takes one argument, a bitmask of the system call functions that are to be traced.
The call is implemented by making modifications to the proc structure. A new integer variable `mask` is added.

- `mask` keeps track of which system calls to trace by setting the corresponding bit "high", in accordance
with the numbering in the file `kernel/syscall.h`.

- In the `allocproc` structure, the mask is initialized to 0, indicating that by default no system calls need to
be tracked. When `trace` is called, it sets the mask of the current process equal to the argument taken as input.

- In the file `kernel/syscall.c`, whenever a system call is performed, an additional piece of code checks, based
off the value of `mask`, whether the current system call needs to be tracked or not. In the former case, it prints
the required information about the system call after it has executed.

- The `strace` function is a command line function and is implemented by simply executing trace at the start of the
process and thereafter calling the exec function for the arguments after the first two ones, which denote the name
of the call, strace and the second argument contains the bitmask.

### System Call 2: `sigalarm` and `sigreturn`

- The sigalarm system call informs a process as it takes CPU time. It takes two arguments, `<interval>` and `<handler>`,
and calls the function `handler` after every `interval` ticks of the CPU.

- This system call is implemented by further modifying the proc structure and adding new variables, `alarm`, `alarmTime`,
`interruptFunction`, `tickCount`.

- `alarm` stores whether the sigalarm function is called or not.

- `alarmTime` stores the duration after which the handler needs to be called.

- `interruptFunction`, as the name suggests, stores the function pointer handler, and is initialized to 0 in `allocproc`.

- `tickCount` stores the number of ticks that the process has currently used up. When `tickCount` equals `alarmTime`,
it is set back to 0, and the program counter of the calling process is set to `interruptFunction`.

- The modified code is largely present in `kernel/trap.c` inside the function `usertrap`.

## Specification 2: Scheduling

- We modify the `Makefile` so that we can take the scheduler argument and use desired scheduler.

### Benchmarking of Schedulers (CPUS=1)

 | OPTION |         Scheduler          |  rtime  |  wtime  |
 | :----: | :-----------------------:  | :-----: | :-----: |
 |   RR   |        Round Robin         |   20    |   182   |
 |  FCFS  |  First Come First Serve    |   41    |   185   |
 |  LBS   |  Lottery Based Scheduler   |   21    |   186   |
 |  PBS   | Priority Based Scheduler   |   20    |   143   |
 |  MLFQ  | Multi-Level Feedback Queue |         |         |

### Round Robin

- Round Robin is the default scheduling protocol already implemented in xv6.

### First Come First Serve

- 
