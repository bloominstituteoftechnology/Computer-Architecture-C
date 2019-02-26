#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  int address = 0;
  FILE *file = fopen(filename, "r");
  char data[1024];

  if (file == NULL)
  {
    printf("File doesnt exist");
    exit(1);
  }

  while (fgets(data, sizeof(data), file) != NULL)
  {
    char *ptr;
    unsigned char ret = strtol(data, &ptr, 2);

    if (ptr == data)
    {
      continue;
    }

    cpu_ram_write(cpu, ret, address++);
  }
  fclose(file);
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
    unsigned char IR = cpu->ram[cpu->PC];
    // 2. Figure out how many operands this next instruction requires
    unsigned char operand0 = cpu->ram[cpu->PC + 1];
    unsigned char operand1 = cpu->ram[cpu->PC + 2];
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
    case LDI:
      cpu->reg[operand0] = operand1;
      cpu->PC += 3;
      break;
    case PRN:
      printf("%u\n", cpu->reg[operand0]);
      cpu->PC += 2;
      break;
    case HLT:
      running = 0;
      break;
    default:
      printf("command not recognized, double check input\n");
      exit(1);
    }
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
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));
}

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

void cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}