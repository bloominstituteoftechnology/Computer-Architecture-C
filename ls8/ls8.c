#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char **argv)
{
  struct cpu cpu;
  // 
   if (argc != 2) {
     // error handling
     fprintf(stderr, "usage: ls8 filname.ls8\n");
     return 1;
   }

  cpu_init(&cpu);
  // if it is equal to 2 then we can pass argv here
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}