#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
  // you can look in argv[1] for the name of the file to load.
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}