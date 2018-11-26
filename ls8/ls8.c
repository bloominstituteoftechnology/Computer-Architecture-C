#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char **arg)
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(arg, &cpu);
  cpu_run(&cpu);

  return 0;
}