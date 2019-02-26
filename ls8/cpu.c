#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
      // From print8.ls8
      0b10000010, // LDI R0,8
      0b00000000,
      0b00001000,
      0b01000111, // PRN R0
      0b00000000,
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
/*
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
*/

// cpu_ram_read
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

// cpu_ram_read
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
  unsigned char IR;
  unsigned char operandA;
  unsigned char operandB;
  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    if (IR >= 0b10000000)
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1);
      operandB = cpu_ram_read(cpu, cpu->PC + 2);
    }
    else if (IR >= 0b01000000)
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1);
    }
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (IR)
    {
    case LDI:
      cpu_ram_write(cpu, operandA, operandB);
      cpu->registers[operandA] = cpu->ram[operandA];
      cpu->PC += 3;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      cpu->PC += 2;
      break;
    case HLT:
      exit(1);
      break;
    default:
      break;
    }

    // IR = cpu_ram_read(cpu, cpu->PC);
    // 6. Move the PC to the next instruction.
  }
  // printf("%d\n", IR);
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));
}
