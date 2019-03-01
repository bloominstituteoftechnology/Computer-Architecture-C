#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char **argv)
{



  struct cpu cpu;


  if (argc != 2) {
    fprintf(stderr, "pass in a valid file only");
    return(1);
  }

  //Gets the filename that was passed in from the command line
  char *filename = argv[1];

  cpu_init(&cpu);
  cpu_load(&cpu, filename);
  cpu_run(&cpu);

  return 0;
}
