#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;
  char *filename = argv[1];
  if (argc > 1) {
    cpu_init(&cpu);
    cpu_load(&cpu, filename);
    cpu_run(&cpu);
  } else {
    printf("ls8: please provide a file to run\n");
}
  
  return 0;
}