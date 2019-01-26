#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "usage: ./ls8 filename.ls8\n");
    return 1;
  }
  
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv);
  cpu_run(&cpu);

  return 0;
}