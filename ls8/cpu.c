#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE * fp = fopen(file, "r");
  if (fp == NULL) {
    fprintf(stderr, "There was an error opening the file \n");
    exit(1);
  }

  char str[128];
  char instruction[8];
  int index = 0;
  char databuff[256];

  while (fgets(str, sizeof(str), fp) != NULL) 
  {
    if (sscanf(str, "%s", instruction) == 1) 
    {
      if (str[0] == '0' || str[0] == '1') 
      {
        databuff[index++] = strtoul(instruction, NULL, 2);
      }
    }
  }

  int address = 0;

  for (int i = 0; i < index; i++)
  {
    cpu->ram[address++] = databuff[i];
  }
};

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) 
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char value) 
{
  cpu->ram[address] = value;
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
