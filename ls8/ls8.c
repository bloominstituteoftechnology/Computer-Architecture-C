#include <stdio.h>
#include "cpu.h"
#include <stdio.h>

/**
 * Main
 */
int main(void)
{
  printf("Starting Main....\n");
  
  //Instantiate the cpu structure:
  struct cpu cpu;

  //Call the initialize function on address of cpu:
  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}