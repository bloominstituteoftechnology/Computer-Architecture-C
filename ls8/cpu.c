#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char value)
{
  cpu->ram[index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp = fopen(argv[1], "r");
  char line[8192];

  if (fp == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }

  int address = 0;

  while (fgets(line, sizeof line, fp) != NULL)
  {
    cpu->ram[address] = strtol(line, NULL, 2);
    address++;
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
    cpu->registers[regA] *= cpu->registers[regB];
    break;
  case ALU_ADD:
    cpu->registers[regA] += cpu->registers[regB];
    break;
  case ALU_CMP:
    if (cpu->registers[regA] == cpu->registers[regB])
    {
      cpu->FL |= 1UL << 0;
    }
    else
    {
      cpu->FL &= ~(1UL << 0);
    }
    if (cpu->registers[regA] > cpu->registers[regB])
    {
      cpu->FL |= 1UL << 1;
    }
    else
    {
      cpu->FL &= ~(1UL << 1);
    }
    if (cpu->registers[regA] < cpu->registers[regB])
    {
      cpu->FL |= 1UL << 2;
    }
    else
    {
      cpu->FL &= ~(1UL << 2);
    }
    break;
  case ALU_AND:
    cpu->registers[regA] = cpu->registers[regA] & cpu->registers[regB];
    break;
  case ALU_OR:
    cpu->registers[regA] = cpu->registers[regA] | cpu->registers[regB];
    break;
  case ALU_XOR:
    cpu->registers[regA] = cpu->registers[regA] ^ cpu->registers[regB];
    break;
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
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    int pc_increment = (instruction >> 6) + 1;

    switch (instruction)
    {
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    case PUSH:
      cpu->registers[7]--;
      cpu_ram_write(cpu, cpu->registers[7], cpu->registers[operandA]);
      break;
    case POP:
      cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[7]);
      cpu->registers[7]++;
      break;
    case CALL:
      cpu->registers[7]--;
      cpu_ram_write(cpu, cpu->registers[7], cpu->PC + 2);
      cpu->PC = cpu->registers[operandA];
      pc_increment = 0;
      break;
    case RET:
      cpu->PC = cpu_ram_read(cpu, cpu->registers[7]);
      cpu->registers[7]++;
      pc_increment = 0;
      break;
    case CMP:
      alu(cpu, ALU_CMP, operandA, operandB);
      break;
    case JMP:
      cpu->PC = cpu->registers[operandA];
      pc_increment = 0;
      break;
    case JEQ:
      if ((cpu->FL >> 0) & 1U)
      {
        cpu->PC = cpu->registers[operandA];
        pc_increment = 0;
      }
      break;
    case JNE:
      if (!((cpu->FL >> 0) & 1U))
      {
        cpu->PC = cpu->registers[operandA];
        pc_increment = 0;
      }
      break;
    case AND:
      alu(cpu, ALU_AND, operandA, operandB);
      break;
    case OR:
      alu(cpu, ALU_OR, operandA, operandB);
      break;
    case XOR:
      alu(cpu, ALU_XOR, operandA, operandB);
      break;
    case HLT:
      running = 0;
      break;
    default:
      exit(1);
    }
    cpu->PC += pc_increment;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->FL = 0b00000000;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  cpu->registers[7] = 0xF4;
}
