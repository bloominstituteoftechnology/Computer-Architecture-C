#include "cpu.h"

#define DATA_LEN 6

/**
 * cpu_ram_read/cpu_ram_write
 */
unsigned char cpu_ram_read(struct cpu *cpu, int idx)
{
  return cpu->ram[idx];
}

unsigned char cpu_ram_write(struct cpu *cpu, int idx, unsigned char val)
{
  cpu->ram[idx] = val;
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
    //PC
    int pc = cpu->PC;

    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, pc); //instruction register
    unsigned char operandA = cpu_ram_read(cpu, pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, pc + 2);

    // 2. switch() over it to decide on a course of action.
    switch (IR)
    {
    case (LDI):
      cpu->reg[operandA] = operandB;
    case (PRN):
      printf("\nValue at register %d: %d\n", operandA, cpu->reg[operandA]);
    case (HLT):
      running = 0;
      break;
    }
    // 3. Do whatever the instruction should do according to the spec.

    // 4. Move the PC to the next instruction.
    cpu->PC += (IR >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}
