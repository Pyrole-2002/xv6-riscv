#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

uint64
main( int argc, char *argv[] )
{
   int Mask = (int)atoi( argv[1] );
   // Setting the process mask as mask input.
   trace(Mask);
   exec( argv[2], &argv[2]);

   exit(0);
}
