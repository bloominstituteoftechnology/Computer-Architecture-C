#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{

  // char array[200];
  // FILE *fPointer;
  // fPointer = fopen("examples/mult.ls8", "r");

  // while(!feof(fPointer)) {
  //   fgets(array, 200, fPointer);
  //   puts(array);
  // }
  // fclose(fPointer);
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}