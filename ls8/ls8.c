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

  while (fgets(buffer, 10, f))
  {
    //fgets(buffer, 10, f);
    char *ptr;
    printf("buffer: %s\n", buffer);
    long instruction = strtol(buffer, &ptr, 2);
    //data[index] = buffer;
    printf("the number: %ld\n", instruction);
  }
  // *fgets(buffer, 256, f);
  printf("\n------\n%s", buffer);
  //printf("data: %s\n", data[0]);

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}