#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])  // main taking argc and argv which is an array of strings
{
  struct cpu cpu;

  if (argc != 2) {  // if argc is not equal to 2 
    fprintf(stderr, "usage: ls8 filename\n"); // print standard error
    exit(1);  // exit with code 1
  }

  char *fiename = argv[1];

  cpu_init(&cpu);
  cpu_load(&cpu, filename); // pass in also the filename in cpu load
  cpu_run(&cpu);

  return 0;
}
