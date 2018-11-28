#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DEBUG 0
#define DATA_LEN 6
#define SR 5

void stack_push(struct cpu *cpu, unsigned char val)
{
  cpu->reg[SR]--;
  cpu->ram[cpu->reg[SR]] = val;
}
unsigned char stack_pop(struct cpu *cpu)
{
  unsigned char value = cpu->ram[cpu->reg[SR]];
  cpu->reg[SR]++;
  return value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  // char data[DATA_LEN] = {
  //     // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT
  // };
  FILE *fp = fopen(filename, "r");
  int address = 0;
  char c[8192];
  while (fgets(c, sizeof c, fp) != NULL)
  {
    cpu->ram[address++] = strtoul(c, NULL, 2);
  }
  fclose(fp);
  ;

  // TODO: Replace this with something less hard-coded
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

    // TODO: implement more ALU ops
  }
}
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
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
      if (DEBUG) {
          printf("HLT\n");
        }
        running = 0;
      break;
    case LDI:
     if (DEBUG) {
          printf("LDI\n");
        }
        cpu->reg[operandA] = operandB;
      break;
    case PRN:
      if (DEBUG) {
          printf("PRN\n");
        }
        printf("%d\n", cpu->reg[operandA]);
      break;
    case MUL:
      if (DEBUG) {
          printf("MUL\n");
        }
        alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case PUSH:
      if (DEBUG) {
          printf("PUSH\n");
        }
        cpu->ram[--cpu->reg[7]] = cpu->reg[operandA];
      break;
    case POP:
      if (DEBUG) {
          printf("POP\n");
        }
        cpu->reg[operandA] = cpu->ram[cpu->reg[7]++];
      break;
    default:
      printf("%d", IR);
      printf("Default case reached.\n");
      break;
    }
    // 4. Move the PC to the next instruction.
    cpu->PC += 1 + (IR >> 6);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, 7);
  cpu->reg[7] = 0xF4;
  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->ram, 0, 256);
}
