#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if (argc > 2 || argc < 2)
  {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *fp;
  char line[1024];

  fp = fopen(argv[1], "r");

  while (fgets(line, sizeof line, fp) != NULL)
  {
    printf("%s", line);

    if (line[0] == '\n' || line[0] == '#')
    {
      continue;
    }

    unsigned char b;
    b = strtoul(line, NULL, 2);

    printf("%02X\n", b);
  }

  fclose(fp);
  /*
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);
  */
  return 0;
}
