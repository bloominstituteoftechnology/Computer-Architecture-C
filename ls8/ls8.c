#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  if(argc < 2) {
		fprintf(stderr, "Provide a file to read! USAGE: ./ls8 filename\n");
		exit(1);
	}


  // init a CPU struct
  CPU cpu;

  char *filename = argv[1];

  cpu_init(&cpu);

  // give it a point to the file
  cpu_load(&cpu, filename);

  cpu_run(&cpu);

  return 0;
}
