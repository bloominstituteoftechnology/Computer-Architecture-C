#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6
#define SP 7

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->reg[SP]--;
  cpu->ram[cpu->reg[SP]] = val;
}

unsigned char cpu_sp(struct cpu *cpu)
{
  unsigned char ret = cpu->ram[cpu->reg[SP]];
  cpu->reg[SP]++;

  return ret;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char address = 0x00;

  if (fp == NULL)
  {
    fprintf(stderr, "\nError opening file.\n");
    exit(1);
  }

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    char *ptr;
    unsigned char ret = strtoul(line, &ptr, 2);

    if (ptr == line)
    {
      continue;
    }
    cpu_ram_write(cpu, ret, address++);
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] *= cpu->reg[regB];
    break;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char operand_a;
  unsigned char operand_b;
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.

    // TODO
    unsigned char IR = cpu->ram[cpu->PC];
    unsigned int number_of_operands = (IR >> 6);
    if (number_of_operands == 2)
    {
      operand_a = cpu->ram[cpu->PC + 1];
      operand_b = cpu->ram[cpu->PC + 2];
    }
    else if (number_of_operands == 1)
    {
      operand_a = cpu->ram[cpu->PC + 1];
    }
    switch (IR)
    {
    case LDI:
      cpu->reg[operand_a] = operand_b;
      break;
    case PRN:
      printf("%d\n", cpu->reg[operand_a]);
      break;
    case MUL:
      alu(cpu, ALU_MUL, operand_a, operand_b);
    case PUSH:
      cpu_push(cpu, cpu->reg[operand_a]);
      break;
    case POP:
      cpu->reg[operand_a] = cpu_sp(cpu);
      break;
    case HLT:
      running = 0;
    default:
      break;
    }
    cpu->PC += number_of_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
}