#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  if (argc == 1)
  {
    printf("Please give me a file to load!\n");
    exit(1);
  }

  char *file = argv[1];

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}