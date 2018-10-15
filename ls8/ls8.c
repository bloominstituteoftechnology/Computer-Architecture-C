#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  printf("Right at the beginning\n");
  struct cpu cpu;
  printf("About to initialize\n");
  cpu_init(&cpu);
  printf("About to load\n");
  cpu_load(&cpu);
  printf("About to run\n");
  cpu_run(&cpu);

  return 0;
}