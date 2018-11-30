#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;
  char *filename = argv[1];
  if(argc < 2) {
    fprintf(stderr, "Two arguments expected.");
    exit(1);
  }
  cpu_init(&cpu);
  cpu_load(filename, &cpu);
  cpu_run(&cpu);

  return 0;
}