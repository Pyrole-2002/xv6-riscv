# REPORT Assignment - 4
### Authors: Aryan Bansal, Hardik Sharma

This report explains briefly the changes and improvements made to the existing xv6 riscv operating system.

## Running the modified xv7 riscv

- `cd` to the directory containing `Makefile` and run `make qemu SCHEDULER=[OPTIONS] CPUS=[1-8]`.
The `SCHEDULER` and `CPUS` flags are optional. If not specified, SCHEDULER=RR and CPUS=8.

## Specification 1: System Calls

### System Call 1: `trace`

- Add content here

### System Call 2: `sigalarm` and `sigreturn`

- Add content here

## Specification 2: Scheduling

### Benchmarking of Schedulers (CPUS=1)

 | OPTION |         Scheduler          |  rtime  |  wtime  |
 | :----: | :-----------------------:  | :-----: | :-----: |
 |   RR   |        Round Robin         |   15    |   160   |
 |  FCFS  |  First Come First Serve    |   31    |   143   |
 |  LBS   |  Lottery Based Scheduler   |         |         |
 |  PBS   | Priority Based Scheduler   |   16    |   132   |
 |  MLFQ  | Multi-Level Feedback Queue |   15    |   151   |

### Round Robin

- Round Robin is the default scheduling protocol already implemented in xv6.

### First Come First Serve

- 
