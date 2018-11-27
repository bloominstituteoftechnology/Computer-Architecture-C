#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{

  FILE *fp;
  struct cpu cpu;

  if(argc < 2){
    printf("You need run the ./ls8 with another argument");
    exit(1);
  } else if(argv[0] == NULL || argv[1] == NULL){
    printf("You need run the ./ls8 with another argument");
    exit(1);
  } else {
    fp = fopen(argv[1], "r");
    cpu_init(&cpu);
    cpu_load(&cpu, &fp);
    cpu_run(&cpu);
  }

  return 0;
}