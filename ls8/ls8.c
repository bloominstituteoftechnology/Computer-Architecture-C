#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if (argc < 2 || argc > 2) {
    printf("Usage: ./ls8 <<prog_name>>\n");
    exit(-1);
  }

  char *prog_name = argv[1];

  struct cpu *cpu = malloc(sizeof(struct cpu));
  cpu_init(cpu);
  cpu_load(cpu, prog_name);
  cpu_run(cpu);
  cpu_free(cpu);

  return 0;
}