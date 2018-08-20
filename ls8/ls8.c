#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  printf("main 0");
  struct cpu cpu;

  printf("main 1");
  cpu_init(&cpu);
  printf("main 2");
  cpu_load(&cpu);
  printf("main 3");
  cpu_run(&cpu);
  printf("main 4");

  return 0;
}