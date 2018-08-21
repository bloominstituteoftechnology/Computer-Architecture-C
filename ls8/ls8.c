#include <stdio.h>
#include <errno.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  
  if (argc <= 1) {
    return fprintf(stderr, "Format for input is './ls8' and '/path/to/file'\n");
  }
  
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, *argv);
  cpu_run(&cpu);

  return 0;
}