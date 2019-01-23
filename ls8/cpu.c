#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  // TODO: Replace this with something less hard-coded

  // need file
  FILE *fp = fopen(file, "r");

  // if file does not exist
  if (fp == NULL)
  {
    printf("File does not exist");
    exit(1);
  }
  else
  {
    // create a var that will hold line of file
    char file_line[1024];
    int address = 0;
    // while not end of file
    while (fgets(file_line, sizeof(file_line), fp) != NULL)
    {
      // cycle through ram and store value of line into ram
      cpu->ram[address] = strtol(file_line, NULL, 2);
      address++;
    }
  }
  // close file
  fclose(fp);
}

// cpu read ram
// would have to take a cpu struct and an index for ram
void cpu_ram_read(struct cpu *cpu, unsigned char memadr)
{
  return cpu->ram[memadr];
}

// cpu write ram
// would have to take a cpu struct, an index for ram, and char value
void cpu_ram_read(struct cpu *cpu, unsigned char memadr, unsigned char value)
{
  return cpu->ram[memadr] = value;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
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

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction;
    // 2. Figure out how many operands this next instruction requires
    unsigned int num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // set all values to 0
  cpu->PC = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  cpu->reg[7] = 0xF4;
}
