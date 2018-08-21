#include <stdio.h>
#include <string.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  struct CPU cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}