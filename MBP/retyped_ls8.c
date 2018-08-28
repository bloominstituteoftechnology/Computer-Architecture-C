#include <stdio.h>
#include "retyped_cpu.h"

int main(void)
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}