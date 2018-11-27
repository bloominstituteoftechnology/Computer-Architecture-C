#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
  if (argc==2) {
    cpu_load(&cpu,argv[1]);
  } else {
    cpu_load(&cpu,"./examples/print8.ls8");
  }
  cpu_run(&cpu);

  return 0;
}