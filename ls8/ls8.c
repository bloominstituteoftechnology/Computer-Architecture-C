#include <stdio.h>
#include "cpu.h"

/**
 * Main  -- intializes the cpu
 */
int main(void)
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}