#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char *argv[])
{

  if (argc == 1)
  {
    printf("Please give me a file to load!\n");
    exit(1);
  }

  //load file
  char *file = argv[1];
  char ch, buffer[256];
  char data[256];
  int index = 0;
  FILE *f = fopen(argv[1], "r");

  while ((ch = fgetc(f)) != EOF)
  {
    fgets(buffer, 8, f);

    data[index++] = buffer;
    printf("%s\n", buffer);
  }
  // *fgets(buffer, 256, f);
  printf("%s", buffer);

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}