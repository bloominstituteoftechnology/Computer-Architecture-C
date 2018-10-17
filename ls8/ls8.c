#include <stdio.h>
#include "cpu.h"
#include <stdio.h>

/**
 * Main
 */
int main(int argc, char **argv)
{
  printf("\nStarting Main....\n");
  
  if (argc != 2){
    fprintf(stderr, "usage: ls8 filename.ls8");
    return 1;
  }
  
  //Instantiate the cpu structure:
  struct cpu cpu;

  //Call the initialize function on address of cpu:
  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]); 
  cpu_run(&cpu);

  return 0;
}