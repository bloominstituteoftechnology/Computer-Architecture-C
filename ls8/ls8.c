#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Not enough arguments.\nEx: ./ls8 {file path}\n");
    exit(1);
  }

  FILE *fptr;
  fptr = fopen(argv[1], "r");
  if (!fptr)
  {
    printf("No such file.\n");
    exit(1);
  }
  fclose(fptr);
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}