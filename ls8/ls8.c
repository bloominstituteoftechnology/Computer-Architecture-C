#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  struct cpu cpu;

  char *filename = "./examples/call.ls8";

  cpu_init (&cpu);
  cpu_load(filename, &cpu);
  cpu_run(&cpu);

  return 0;
}