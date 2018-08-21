#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;
  char *filename = "examples/print8.ls8";
  if (argc == 2) {
    filename = argv[1];
  }
  FILE * fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("fopen failed to open the file\n");
    exit(-1);
  }

  char line[128];
  char instruction[8];
  int index = 0;
  char data[256];

  while(fgets(line, sizeof(line), fp) != NULL)
  {
    if(sscanf(line, "%s", instruction) == 1)
    {
      if ( line[0] == '0' || line[0] == '1' ){
        data[index++] = strtoul(instruction, NULL, 2);
      }
    }
  }

  cpu_init(&cpu);
  cpu_load(&cpu, data);
  cpu_run(&cpu);

  return 0;
}