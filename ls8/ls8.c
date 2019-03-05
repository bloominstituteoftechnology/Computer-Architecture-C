#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
//the way this is set up shows us the order that this should be built out
int main(int argc, char *argv[]) //changed from void for command line processing. arg count and array of strings that hold indiv args.
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}

//argv[0] == "./ls8"
//argv[1] == "examples/mult.ls8"