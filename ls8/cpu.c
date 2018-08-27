#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
  cpu->ram[MAR] = MDR;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *argv[])
{
  char line[100];
  int address = 0;
  FILE *fp = fopen(argv[1], "r");
  // printf("%s\n", filename);
  if (fp == NULL)
  {
    fprintf(stderr, "File pointer is  not working");
    exit(1);
  }
  while (fgets(line, sizeof(line), fp))
  {
    char *endchar;
    unsigned long v;
    v = strtoul(line, &endchar, 2);
    if (!(*line == *endchar))
      cpu_ram_write(cpu, address++, v);
  }
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

  case ALU_ADD:
    cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
    break;

  case ALU_DIV:
    cpu->reg[regA] = cpu->reg[regA] / cpu->reg[regB];
    break;

  case ALU_SUB:
    cpu->reg[regA] = cpu->reg[regA] - cpu->reg[regB];
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
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);

    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);

    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);

    printf("TRACE: %02x: %02x\n", cpu->pc, IR);
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // unsigned const char IR = ram->
    // 2. switch() over it to decide on a course of action.
    switch (IR)
    {
    case LDI:
      cpu->reg[operandA] = operandB;
      // cpu->pc += 2;
      break;

    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      // cpu->pc += 2;
      break;

    case HLT:
      running = 0;
      // cpu->pc += 1;
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      // cpu->pc += 2;
      break;

    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      // cpu->pc += 2;
      break;

    case DIV:
      alu(cpu, ALU_DIV, operandA, operandB);
      // cpu->pc += 2;
      break;

    case SUB:
      alu(cpu, ALU_SUB, operandA, operandB);
      // cpu->pc += 2;
      break;

    case PUSH:
      // printf("I am in push");
      cpu->reg[7]--;
      cpu_ram_write(cpu, cpu->reg[7], cpu->reg[operandA]);
      // cpu->pc += 1;
      break;

    case POP:
      // printf("I am in POP");
      // printf("%s POP\n", cpu->reg[operandA]);
      cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[7]);
      cpu->reg[7]++;
      // cpu->pc += 1;
      break;

    case CMP:
      printf("I am in CMP\n");
      if (cpu->reg[operandA] == cpu->reg[operandB])
      {
        cpu->FL = 0b00000001;
        // printf("I am in here one\n");
      }
      if (cpu->reg[operandA] < cpu->reg[operandB])
      {
        cpu->FL = 0b00000100;
        // printf("I am in here two\n");
      }
      if (cpu->reg[operandA] > cpu->reg[operandB])
      {
        cpu->FL = 0b00000010;
        // printf("I am in here three\n");
      }
      // printf("I am done with CMP\n");
      // cpu->pc += 2;
      break;

    case JEQ:
      printf("I am in JEQ\n");
      if ((cpu->FL & 0b001) == 0b000000001)
      {
        cpu->pc = cpu->reg[operandA];
        // cpu->pc += 1;
        cpu->pc -= ((IR >> 6) + 1);
      }
      break;

    case JNE:
      printf("I am in JNE\n");
      if ((cpu->FL & 0b001) == 0b00000000)
      {
        cpu->pc = cpu->reg[operandA];
        cpu->pc -= ((IR >> 6) + 1);
      }
      break;

    case JMP:
      printf("I am in JMP\n");
      cpu->pc = cpu->reg[operandA];
      // cpu->pc += 1;
      cpu->pc -= ((IR >> 6) + 1);
      break;

    default:
      printf("Unknown instruction at %02x: %02x\n", cpu->pc, IR);
      exit(2);
    }
    cpu->pc += (IR >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
  cpu->FL = 0;
  cpu->reg[7] = 0xF4; //cpu_load
  memset(cpu->reg, 0, sizeof(cpu->reg));

  memset(cpu->ram, 0, sizeof(cpu->ram));
  // printf("I am here CPU_INIT \n");
  // FL = 0b00000000;
}
