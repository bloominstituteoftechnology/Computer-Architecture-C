#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char **argv)
{
  struct cpu cpu;
  char *program;

  if (argc < 2) {
    fprintf(stderr, "Please specify which program the LS8 should run.\nUsage: ls8 program\n");
    exit(1);

  } else {
    program = argv[1];

    printf("%s\n", program);

    cpu_init(&cpu);
    cpu_load(&cpu, program);
    cpu_run(&cpu);
  }

  return 0;
}