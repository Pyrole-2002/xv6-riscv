#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define NFORK 5

int main()
{
    int pid;
#ifdef LBS
    pid = fork();
    if (pid < 0)
    {
        exit(0);
    }
    else if (pid == 0)
    {
        // Child
        for (int i = 0; i < 1000 * 500000; i++)
        {
            if (i % 1000000 == 0)
            {
                printf(".");
            }
        }
    }
    else
    {
        // Parent
        for (int i = 0; i < 1000 * 500000; i++)
        {
            if (i % 1000000 == 0)
            {
                printf("#");
            }
        }
    }
    exit(0);
#endif



    int n;
    int wtime, rtime;
    int twtime=0, trtime=0;
    for(n = 0; n < NFORK; n++)
    {
        pid = fork();
        if (pid < 0)
        {
            break;
        }
        if (pid == 0)
        {
            for (volatile int i = 0; i < 10000000000; i++)
            {} // CPU bound process 
            // printf("Process %d finished\n", n);
            exit(0);
        }
        else
        {
            set_priority(80, pid); // Will only matter for PBS, set lower priority for IO bound processes 
        }
    }
    for(; n > 0; n--)
    {
        if(waitx(0,&rtime,&wtime) >= 0)
        {
            trtime += rtime;
            twtime += wtime;
        } 
    }
    printf("Average rtime %d,  wtime %d\n", trtime / NFORK, twtime / NFORK);
    exit(0);
}
