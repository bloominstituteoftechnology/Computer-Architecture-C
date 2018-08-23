#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
// "argc" is the argument count, and argv is an array of strings that hold the individual arguments, 
// starting with the command name itself.
int main(int argc, char *argv[])
{
  struct cpu cpu;

  if (argc < 2)
  {
    fprintf(stderr, "Error: Kindly enter name of file.\n");
    exit(1);
  }

  cpu_load(&cpu, argv[1]);
  cpu_init(&cpu);
  cpu_run(&cpu);

  return 0;
}