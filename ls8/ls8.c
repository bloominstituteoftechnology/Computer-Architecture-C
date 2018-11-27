#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  if (argc != 2) {
      printf("Usage: ./ls8 examples/print8.ls8\n");
      exit(1);
    } else {
      cpu_init(&cpu); //turn on
      cpu_load(&cpu, argv[1]); //load data
      cpu_run(&cpu);  // execute
    }

  return 0;
}