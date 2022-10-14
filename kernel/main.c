#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;
extern int total_tickets;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
    if(cpuid() == 0)
    {
        // Executed by Core-0
        consoleinit();
        printfinit();
        printf("\n");
        printf("xv6 kernel is booting\n");
        printf("\n");
        kinit();         // physical page allocator
        kvminit();       // create kernel page table
        kvminithart();   // turn on paging
        procinit();      // process table
        trapinit();      // trap vectors
        trapinithart();  // install kernel trap vector
        plicinit();      // set up interrupt controller
        plicinithart();  // ask PLIC for device interrupts
        binit();         // buffer cache
        iinit();         // inode table
        fileinit();      // file table
        virtio_disk_init(); // emulated hard disk
        userinit();      // first user process
        // Compiler tries to optimise code
        // __sync_synchronize is used to force compiler to execute all commands above it
        // first and then only move ahead
        __sync_synchronize();
        started = 1;
    }
    else
    {
        // Executed by all other Cores
        // Wait till Core-0 finishes initialising
        while(started == 0);
        __sync_synchronize();
        printf("hart %d starting\n", cpuid());
        kvminithart();    // turn on paging
        trapinithart();   // install kernel trap vector
        plicinithart();   // ask PLIC for device interrupts
    }

    scheduler();        
}
