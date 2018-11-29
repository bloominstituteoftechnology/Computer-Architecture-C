#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
 int main(int argc, char *argv[])
{
  struct cpu cpu;

  char *fn = argv[1];
  if (argc > 1) {
    cpu_init(&cpu);
    cpu_load(&cpu, fn);
    cpu_run(&cpu);
  } else {
    printf("Please specify a file to run\n");
}
  return 0;
}
