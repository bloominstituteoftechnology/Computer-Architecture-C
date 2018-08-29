#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
  struct cpu cpu;
  
  if (argc != 2){
    fprintf(stderr, "Usage: ./ls8 [filename]\n")
  }

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}
