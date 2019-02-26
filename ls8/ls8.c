#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv);
  cpu_run(&cpu);

  return 0;
}