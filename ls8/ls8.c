#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])  //**argv is the same
{
  struct cpu cpu;

  char *filename;

  if (argc == 2)
  {
    filename = argv[1];
  }
  else
  {
    fprintf(stderr, "usage: ./ls8 filename\n");
    exit(1);
  }

  cpu_init(&cpu);
  cpu_load(&cpu, filename);
  cpu_run(&cpu);


  return 0;
}