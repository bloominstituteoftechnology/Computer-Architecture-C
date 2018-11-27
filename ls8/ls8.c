#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;
  char *file = argv[1];

  cpu_init(&cpu);
  cpu_load(&cpu, file);
  cpu_run(&cpu);

  return 0;
}
