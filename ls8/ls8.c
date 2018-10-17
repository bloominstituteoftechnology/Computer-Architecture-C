#include <stdio.h>
#include "cpu.h"
#include <stdio.h>

/**
 * Main
 */
int main(int argc, char *argv[])
{
  printf("\nStarting Main....\n");
  
  if (argv[0]) printf("argv0: %s\n", argv[0]);
  if (argv[1]) printf("args1: %s\n", argv[1]);
  
  //Instantiate the cpu structure:
  struct cpu cpu;

  //Call the initialize function on address of cpu:
  cpu_init(&cpu);
  // cpu_load(&cpu); 

  // Open file and read in every line and place it into RAM:
	FILE *fp;
	char line[1024];

	fp = fopen(argv[1], "r");
  unsigned char mar = 0;

	while (fgets(line, sizeof line, fp) != NULL) {
    // printf("%s", line);

		if (line[0] == '\n' || line[0] == '#') {
			printf("Ignoring this line.\n");
			continue;
		}

		unsigned char assembly_command;
		assembly_command = strtoul(line, NULL, 2);

    //Should write it into Memory
    cpu_ram_write(&cpu, mar++, assembly_command);
    
	}

	fclose(fp);

  cpu_run(&cpu);

  return 0;
}