#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6
#define SP 7

unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char ret = cpu->ram[cpu->reg[SP]];
  cpu->reg[SP]++;
  return ret;
}

// Write the given value to the LS8's Ram at the given address
void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

// Read the value at address and return it
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char addr = 0x00;

  // open the source file
  if (fp == NULL)
  {
    fprintf(stderr, "Trouble opening the file.\n");
    exit(2);
  }

  // read the source file line by line
  while (fgets(line, sizeof(line), fp) != NULL)
  {
    // convert these binary strings to numbers
    char *endptr;
    unsigned char ret = strtoul(line, &endptr, 2);

    if (endptr == line)
    {
      continue;
    }

    cpu_ram_write(cpu, ret, addr++);
  }

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  // TODO
  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] *= cpu->reg[regB];
    break;
  default:
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1;
  unsigned char IR, operandA, operandB;

  while (running)
  {
    // TODO
    IR = cpu_ram_read(cpu, cpu->PC);
    int num_operands = IR >> 6;

    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    if (num_operands == 2)
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1 & 0xff);
      operandB = cpu_ram_read(cpu, cpu->PC + 2 & 0xff);
    }
    else if (num_operands == 1)
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1 & 0xff);
    }

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (IR)
    {
    case LDI:
      cpu->reg[operandA] = operandB;
      break;

    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;

    case HLT:
      running = 0;
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    case PUSH:
      cpu->reg[SP]--;
      cpu->ram[cpu->reg[SP]] = cpu->reg[operandA];

      break;

    case POP:
      cpu->reg[operandA] = cpu->ram[cpu->reg[SP]];
      cpu->reg[SP]++;
      break;

    default:
      printf("Unknown instruction %02x\n", IR);
      exit(3);
    }

    // 6. Move the PC to the next instruction.
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */

void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0x00;
  cpu->reg[SP] = 0xF4;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}