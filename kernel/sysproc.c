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

    argint( 0, &timeInterval );
    argaddr( 1, &functionPointer );
    
    myproc()->tickCount = 0;

    if ( timeInterval < 0 )
    {
        printf("Invalid Arguments to sigalalarm");
        myproc()->alarm = 0;
        return -1;
    }
    else if ( timeInterval == 0 )
    {
        myproc()->alarm = 0;
        return 0;
    }

    //myproc()->Sigtrapframe = myproc()->trapframe;
    
    myproc()->alarm = 1;
    myproc()->alarmTime = timeInterval; 
    myproc()->interruptFunction = functionPointer;
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
    struct proc *currProcess = myproc();
    
    currProcess->Sigtrapframe->kernel_hartid = currProcess->trapframe->kernel_hartid;
    currProcess->Sigtrapframe->kernel_satp = currProcess->trapframe->kernel_satp;
    currProcess->Sigtrapframe->kernel_sp = currProcess->trapframe->kernel_sp;
    currProcess->Sigtrapframe->kernel_trap = currProcess->trapframe->kernel_trap;
    // DO NOT CHANGE the kernel specific info of a process.
    // If the process was executing in another cpu at the time handler was called, 
    // such information should not be changed back.

    *(currProcess->trapframe) = *(currProcess->Sigtrapframe);
    // Returns the state of the registers to before the call of handler function.
    currProcess->alarm = 1;
    return currProcess->trapframe->a0;
}

uint64
sys_waitx(void)
{
    uint64 p, raddr, waddr;
    int rtime, wtime;
    argaddr(0, &p);
    argaddr(1, &raddr);
    argaddr(2, &waddr);
    int ret = waitx(p,&rtime,&wtime);
    struct proc *proc = myproc();
    if (copyout(proc->pagetable, raddr, (char*)&rtime , sizeof(int)) < 0)
    {
        return -1;
    }
    if (copyout(proc->pagetable, waddr, (char*)&wtime , sizeof(int)) < 0)
    {
        return -1;
    }
    return ret;
}

uint64
sys_set_priority(void)
{
    int new_priority, pid;
    argint(0, &new_priority);
    argint(1, &pid);
    return set_priority(new_priority, pid);
}
