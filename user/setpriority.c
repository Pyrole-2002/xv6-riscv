#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int
main(int argc, char** argv)
{
    // Usage:
    // setpriority <priority> <pid>
    if (argc != 3)
    {
        printf("setpriority: wrong syntax\n");
        exit(1);
    }
    int new_priority = atoi(argv[1]);
    int pid = atoi(argv[2]);
    printf("Priority of [%d] : %d -> %d\n", pid, set_priority(new_priority, pid), new_priority);
    exit(0);
}
