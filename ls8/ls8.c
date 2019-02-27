#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Wrong number of arguments.\nUsage: ./ls8 {file path from current directory}\n");
    exit(1);
  }

  FILE *fptr;
  if ((fptr = fopen(argv[1], "r")) == NULL)
  {
    fprintf(stderr, "Error opening file or no such file.\n");
    exit(1);
  }
  fclose(fptr);
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}