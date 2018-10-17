#include "cpu.h"
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#define DATA_LEN 6

void cpu_load(struct cpu *cpu, char *filename)
{
  char pathname[30] = "./examples/";
  char line[1024];
  int address = 0;
  strcat(pathname, filename);
  strcat(pathname, ".ls8");
  FILE *fp;
  fp = fopen(pathname, "r");

  if (fp == NULL)
  {
    puts("couldnt open file");
    exit(0);
  }
  while (fgets(line, sizeof line, fp) != NULL)
  {
    if (line[0] == '\n' || line[0] == '#')
    {
      continue;
    }
    unsigned char b;
    b = strtoul(line, NULL, 2);
    cpu->ram[address++] = b;
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  // unsigned char index1 = cpu->reg[regA];
  // unsigned char index2 = cpu->reg[regB];
  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
    break;
  }
}
char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, int index, char value)
{
  cpu->ram[index] = value;
}
/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1));
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 2));
    int shiftIndex = (IR >> 6) + 1;
    printf("TRACE: %02X: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB);
    switch (IR)
    {
    case LDI:
      cpu->reg[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case HLT:
      running = 0;
      break;
    case PUSH:
      cpu->SP--;
      *cpu->SP = cpu->reg[operandA];
      break;
    case POP:
      cpu->reg[operandA] = *cpu->SP;
      cpu->SP++;
      break;
    }
    cpu->PC += shiftIndex;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  cpu->reg[7] = 0xF4;
  cpu->SP = &cpu->ram[245];
}
