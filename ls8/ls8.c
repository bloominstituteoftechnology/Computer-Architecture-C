#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */

// change args to (int argc, char *argv[]); see README lines 173-183
// `argv[1]` should give the name of the file to load

int main(int argc, char *argv[]) {
  if (argc == 2) {
    struct cpu cpu;
    cpu_init(&cpu);
    cpu_load(&cpu, argv);  // *** NOTE: if changing args for cpu_load, you must change args where that function is called in cpu.h and cpu.c
    cpu_run(&cpu);
    return 0;
  }
  printf("Invalid number of arguments entered. User must provide a file name \n");
  return 0;
}