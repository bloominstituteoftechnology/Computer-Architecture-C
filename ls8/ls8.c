#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  char fileName[] = "./examples/mult.ls8";
  struct cpu cpu;
  cpu_init(&cpu);
  cpu_load(&cpu, fileName);
  cpu_run(&cpu);

  return 0;
}