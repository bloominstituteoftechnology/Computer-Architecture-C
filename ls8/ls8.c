#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  struct cpu cpu;

  cpu_init(&cpu);
  // test line to see if cpu_init is working. comment out cpu_load and cpu_run
  // printf("%c\n", cpu->PC)
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}