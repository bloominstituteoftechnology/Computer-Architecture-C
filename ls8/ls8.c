#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

   if (argc < 2) {
    printf("Error");
    exit(2);
  } else {
    cpu_init(&cpu);
    cpu_load(&cpu, argv);
    cpu_run(&cpu);
  }

  return 0;
}