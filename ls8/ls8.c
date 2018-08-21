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
  char databuff[256];

  while(fgets(line, sizeof(line), fp) != NULL)
  {
    if(sscanf(line, "%s", instruction) == 1)
    {
      if ( line[0] == '0' || line[0] == '1' ){
        databuff[index++] = strtoul(instruction, NULL, 2);
        // printf("databuff[%d] = %lu\n", index-1, strtoul(instruction, NULL, 2));
      }
    }
  }

  char data[index];
  for (int i = 0; i < index; i++)
  {
    data[i] = databuff[i];
  }
  
  // printf("Length of data: %lu\n", ( sizeof(data)/sizeof(data[0]) ));

  cpu_init(&cpu);
  cpu_load(&cpu, data, index);
  cpu_run(&cpu);

  return 0;
}