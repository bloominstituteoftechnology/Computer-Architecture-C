#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  struct cpu my_cpu;

  cpu_init(&my_cpu);
  cpu_load(&my_cpu);
  cpu_run(&my_cpu);

  return 0;
}