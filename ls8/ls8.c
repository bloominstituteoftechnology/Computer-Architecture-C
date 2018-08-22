#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
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

  if (argc != 2) {
    fprintf(stderr, "usage: ls8 filename\n");
    exit(1);
  }

  char *filename = argv[1];

  cpu_load(&cpu, filename);
  cpu_init(&cpu);
  cpu_run(&cpu);

  return 0;
}