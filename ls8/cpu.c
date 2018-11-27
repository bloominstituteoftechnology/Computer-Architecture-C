#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value)
{
  cpu->ram[index] = value;
}
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
    // 1. Get the value of the current instruction (at the address PC).
    unsigned char curr_instruct = cpu_ram_read(cpu, cpu->PC); //LDI
    // 2. switch() over it to decide on a course of action.

    unsigned char value1 = cpu_ram_read(cpu, cpu->PC + 1); // value at R0
    unsigned char value2 = cpu_ram_read(cpu, cpu->PC + 2); // 8

    switch (curr_instruct)
    {

    case LDI:

      cpu->reg[value1] = value2;

      cpu->PC += 3; //PRN

      break;

    case PRN:
      printf("Printing %u\n", cpu->reg[value1]);
      cpu->PC += 2;
      break;

    case HLT:
      printf("Halting program\n");
      exit(0);
      break;

    default:
      exit(1);
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));

  // TODO: Zero registers and RAM
}
