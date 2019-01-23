#include <stdio.h>
#include <string.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, int argv[])
{
  if (argc != 2) {
    printf("Usage: ./ls8 filename \n");
    return 1;
  }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}