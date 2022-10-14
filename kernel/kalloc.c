// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.
uint64 addressMap[ 32*1024 ] = {0};

struct run
{
    // Pointer to the next 4K-Block
    struct run *next;
};

struct
{
    // Associating a lock to freelist
    struct spinlock lock;
    // Pointer to Linked-List of 4k Pages
    struct run *freelist;
} kmem;


struct spinlock reff;

void
kinit()
{
    for( int i = 0; i < 32*1024; i++ )
        addressMap[i] += 1;

    initlock(&kmem.lock, "kmem");
    freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
    char *p;
    p = (char*)PGROUNDUP((uint64)pa_start);
    for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    {
        kfree((void*)p);
    }
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
    struct run *r;

    if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    {
        panic("kfree");
    }

    int index = PGROUNDDOWN((uint64)pa) - KERNBASE;
    index = index / PGSIZE;
    acquire(&reff);
    if ( index < 0 || index > 32*1024 )
    {
        release(&reff);
        return;
    }

    if( addressMap[index] <= 0 )
    {
        printf("Invalid Page Free Request.\n");
        release(&reff);
        return;
    }
    else
    {
        addressMap[index]--;
    }

    if ( addressMap[index] == 0 ) 
    {
        // Fill with junk to catch dangling refs.
        memset(pa, 1, PGSIZE);

        r = (struct run*)pa;

        acquire(&kmem.lock);
        r->next = kmem.freelist;
        kmem.freelist = r;
        release(&kmem.lock);
    }
    release(&reff);
}

void pageRef( void* page )
{
    int index = PGROUNDDOWN( (uint64)page - KERNBASE );
    index = index / PGSIZE;
    acquire(&reff);
    if ( index < 0 || index >= 32*1024 )
    {
        release(&reff);
        return;
    }

    addressMap[index]++;
    release(&reff);
    return ;
}

void *
kalloc(void)
{
    struct run *r;

    acquire(&kmem.lock);
    r = kmem.freelist;
    
    if(r)
        kmem.freelist = r->next;
    
    release(&kmem.lock);

    if(r)
    {
        memset((char*)r, 5, PGSIZE); // fill with junk
        pageRef( (void*) r );
        // Increasing the number of processes that are currently using this page.
    }
    return (void*)r;
}

