#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  printf("--init--\n");
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}