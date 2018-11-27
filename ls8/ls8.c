#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu,"./examples/print8.ls8");
  cpu_run(&cpu);

  return 0;
}