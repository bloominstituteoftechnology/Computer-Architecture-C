#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  if(argc != 2) {
    fprintf(stderr, "Usage: ls8 file.ls8\n");
    return 1;
  }

  return 0;
}