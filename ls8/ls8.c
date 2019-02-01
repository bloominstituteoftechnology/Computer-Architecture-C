#include <stdio.h>
#include "cpu.h"

// going to need to pass in argv and maybe argc need to study the files more. 
int main(int filename)
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, filename);
  cpu_run(&cpu);

  return 0;
}
