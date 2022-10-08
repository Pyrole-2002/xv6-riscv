#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_trace(void)
{
    Bitmask mask;
    argint( 0, &mask);
    myproc()->mask = mask;
    return 0;
}

uint64
sys_sigalarm(void)
{
    int timeInterval;
    uint64 functionPointer;

    myproc()->Sigtrapframe = myproc()->trapframe;

    argint( 0, &timeInterval );
    argaddr( 1, &functionPointer );
    
    myproc()->alarm = 1;
    myproc()->alarmTime = timeInterval; 

    // if( ticks == myproc()->alarmTime );
    //     myproc()->trapframe->epc = functionPointer;
    // The execution of the program woould start from the address in functionPointer.
    // Since the handler does not consist of any arguments or return value, registers need not be updated.

    // After this, the function would anyways go to sigreturn where it would reset the program counter.
    return 0;
}

uint64
sys_sigreturn(void)
{
    myproc()->trapframe = myproc()->Sigtrapframe;
    // Returns the state of the registers to before the call of handler function.
    return 0;
}
