#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    FILE *fp;
    int address = 0;
    char line[1024];

    // open file
    fp = fopen( filename, 'r');
    if (fp == NULL) {
      printf("Could not print file");
      return
    }

    while (fgets(line, sizeof line, fp) != NULL) {
      char *end_of_instr;
      unsigned char *data;

      data = strtol(line, &end_of_instr, 2);
    }
    if(){

    } else {
        cpu->ram[address++] = data[i];
    }
    // fscanf(fp, '%s', line);
    
    // if ( strchr(line, '#') != NULL) {
    
    // }
  };

  int address = 0;

  

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}
