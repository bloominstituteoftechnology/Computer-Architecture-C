#include <stdio.h>
#include "cpu.h"

/**
 * Main
 * argc - argument count
 * argv - an array of strings that hold the individual arguments
 */
int main(int argc, char *argv[])
{
  // check to make sure the user has put a command line argument where you
  // expect, and print an error and exit if they didn't
  if (argv[1] == NULL)
  {
    printf("ERR: expected 2 arguments.\n");
    exit(1);
  }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}