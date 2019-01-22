#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu); // load the specified program into the CPUs RAM
  cpu_run(&cpu); // run the program

  return 0;
}
