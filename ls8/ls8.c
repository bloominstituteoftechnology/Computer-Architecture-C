#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */

// argc is the argument count, and argv is an array of strings that hold
// the individual arguments, starting with the command name itself.
// If the user runs ./ls8 examples/mult.ls8, the values in argv will be:
// argv[0] == "./ls8"
// argv[1] == "examples/mult.ls8" .................Training Kit
int main(int argc, char **argv)
{
  struct cpu cpu;
  // 
   if (argc != 2) {
     // error handling/ usage mesage so user knows what to do
     fprintf(stderr, "usage: ls8 filname.ls8\n");
     return 1;
   }

  cpu_init(&cpu);
  // if it is equal to 2 then we can pass argv here
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}