#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6
#define DEBUG 0

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  int address = 0;
  char line[32];
  char *ptr;
  while (fgets(line, sizeof line, fp) != NULL)
  {
    cpu->ram[address++] = strtoul(line, &ptr, 2);
#if DEBUG
    printf("Line that is not used: %s\n", ptr);
#endif
  }
  fclose(fp);
#if DEBUG
  for (int i = 0; i < 64; i++)
  {
    printf("RAM at index %d: %d\n", i, cpu->ram[i]);
  }
#endif
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
    break;
  case ALU_ADD:
    cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
    break;
  case ALU_CMP:
    if (cpu->reg[regA] < cpu->reg[regB])
      cpu->FL += 0b100;
    if (cpu->reg[regA] > cpu->reg[regB])
      cpu->FL += 0b010;
    if (cpu->reg[regA] == cpu->reg[regB])
      cpu->FL += 0b001;
    else
      cpu->FL = 0;
  default:
    printf("Default case reached. No ALU function triggered.\n");

    // TODO: implement more ALU ops
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char item)
{
  cpu->ram[index] = item;
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
    int PC = cpu->PC;
    unsigned char IR = cpu_ram_read(cpu, PC);
    unsigned char operandA = cpu_ram_read(cpu, PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, PC + 2);
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch (IR)
    {
    case HLT:
#if DEBUG
      printf("HLT\n");
#endif
      running = 0;
      break;
    case LDI:
#if DEBUG
      printf("LDI\n");
#endif
      cpu->reg[operandA] = operandB;
      break;
    case PRN:
#if DEBUG
      printf("PRN\n");
#endif
      printf("%d\n", cpu->reg[operandA]);
      break;
    case MUL:
#if DEBUG
      printf("MUL\n");
#endif
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case PUSH:
#if DEBUG
      printf("PUSH\n");
#endif
      cpu->ram[--cpu->reg[7]] = cpu->reg[operandA];
      break;
    case POP:
#if DEBUG
      printf("POP\n");
#endif
      cpu->reg[operandA] = cpu->ram[cpu->reg[7]++];
      break;
    case CALL:
#if DEBUG
      printf("CALL\n");
#endif
      cpu->ram[--cpu->reg[7]] = PC + 2;
      cpu->PC = cpu->reg[operandA] + 1;
#if DEBUG
      printf("What we're setting PC to in CALL: %d\n", cpu->reg[operandA]);
#endif
      break;
    case RET:
#if DEBUG
      printf("RET\n");
#endif
      cpu->PC = cpu->ram[cpu->reg[7]++];
      break;
    case ADD:
#if DEBUG
      printf("ADD\n");
#endif
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    case CMP:
      alu(cpu, ALU_CMP, operandA, operandB);
      break;
    case JMP:
      cpu->PC = cpu->reg[operandA];
      break;
    case JEQ:
      break;
    case JNE:
      break;
    default:
      printf("Instruction number: %d\n", IR);
      printf("Default case reached. Command invalid.\n");
      break;
    }
    // 4. Move the PC to the next instruction.
    if (!((IR >> 4) & 00000001))
    {
      cpu->PC += 1 + (IR >> 6);
    }

#if DEBUG
    sleep(1);
#endif
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special reg
  // TODO: Zero reg and RAM
  memset(cpu->reg, 0, 7);
  cpu->reg[7] = 0xF4;
  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->ram, 0, 256);
}