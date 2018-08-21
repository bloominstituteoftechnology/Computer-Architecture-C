#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  printf("main 0: START\n");
  struct cpu cpu;

  printf("main 1\n");
  cpu_init(&cpu);
  printf("main 2\n");
  cpu_load(&cpu);
  printf("main 3\n");
  cpu_run(&cpu);
  printf("\nmain 4: END\n");

  return 0;
}