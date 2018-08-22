#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;
  char *file;

  if (argc ==2) {
    file = argv[1];
  } else {
    fprintf(stderr, "./ls8 examples/file.ls8\n");
    exit(1);
  }
  cpu_init(&cpu);
  cpu_load(&cpu, file);
  cpu_run(&cpu);

  return 0;
}