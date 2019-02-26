#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define BUF_SIZE 256

int s_to_binary(const char *s)
{
  return (int)strtol(s, NULL, 2);
}

int countlines(FILE *f)
{
  int nlines = 0;
  char line[BUF_SIZE];

  while (fgets(line, BUF_SIZE, f) != NULL)
  {
    nlines++;
  }

  rewind(f);

  return nlines;
}

/**
 * Main
 */
int main(int argc, char *argv[])
{
  int data[BUF_SIZE];
  int lines;

  if (argc > 1)
  {
    FILE *f = fopen(argv[1], "r");
    char str[BUF_SIZE];
    lines = countlines(f);
    int tmpLines = lines;

    if (f == NULL)
    {
      printf("NULL");
      exit(1);
    }
    while (fgets(str, BUF_SIZE, f) != NULL)
    {
      char *ptr;
      ptr = strchr(str, '#');
      if (ptr != NULL)
      {
        *ptr = '\0';
      }
      int b = s_to_binary(str);
      data[lines - tmpLines] = b;
      tmpLines--;
    }
    fclose(f);
  }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, data, lines);
  cpu_run(&cpu);

  return 0;
}