#include <stdio.h>
#include "cpu.h"
#include <stdlib.h> 

/**
 * Main
 */
int main(int argc, char **argv) //`argc` is the argument count, and `argv` is an array of strings that hold the individual arguments
{
  struct cpu cpu;

  cpu_init(&cpu);

  if(argc == 2)
  {
    cpu_load(&cpu, argv[1]);
    cpu_run(&cpu); 
  }
  else
  {
    printf("No filename was given!\n");
    exit(1);
  }

  return 0;
}