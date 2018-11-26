#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;
if (argc != 2)
  {
    fprintf(stderr, "Usage: LS8 expects 2 inputs\n");
    exit(1);
  }
  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}