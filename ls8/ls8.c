#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(void)
{
  struct cpu *cpu = malloc(sizeof(struct cpu));
  cpu_init(cpu);
  cpu_load(cpu);
  cpu_run(cpu);
  cpu_free(cpu);

  return 0;
}