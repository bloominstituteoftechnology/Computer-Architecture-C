#include <stdio.h>
#include <string.h>
#include "cpu.h"

/**
 * Main
 */
//int main(void)
int main(int argc, char **argv)
{
  struct CPU cpu;
  if (argc < 2) {
    fprintf(stderr, "usage: ./ls8 + example/<programfile> \n");
  }
  else {
  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);
  }



  return 0;
}