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
  if (argc < 2)
  {
    fprintf(stderr, "Error. Please enter filename.\n");
    exit(1);
  }

  // Initializes a CPU structure.
  CPU cpu;

  char *file = argv[1];

  cpu_init(&cpu);

  // Gives a point to the file.
  cpu_load(&cpu, file);

  cpu_run(&cpu);

  return 0;

  // cpu_load(&cpu, argv[1]);
  // cpu_init(&cpu);
  // cpu_run(&cpu);

  return 0;
}