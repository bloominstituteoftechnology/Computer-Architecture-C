#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  char *filename = argv[1];
  if (argc != 2)
  {
    puts("need a program to run");
    exit(0);
  }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, filename);
  cpu_run(&cpu);

  return 0;
}