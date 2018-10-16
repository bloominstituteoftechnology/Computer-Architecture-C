#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{

  // if (argc != 2)
  // {
  //   fprintf(stderr, "usage: ls8 .ls8 file\n");
  //   exit(2);
  // }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}