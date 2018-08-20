#include "cpu.h"

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  const int DATA_LEN = 6;
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
  int PC = 0;

  while (running)
  {
    // === STEP 3 TODO ===
    // 1. Get the value of the current instruction (in address PC).
     unsigned char IR = cpu_ram_read(cpu, PC);
     unsigned char operandA = cpu_ram_read(cpu, PC+1);
     unsigned char operandB = cpu_ram_read(cpu, PC+2);

    // 2. switch() over it to decide on a course of action.
    switch(IR)
    {
      // STEP 5
      case LDI:
      // STEP 4
      case HLT:
      // STEP 6
      case PRN:
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

  // TODO: Zero registers and RAM
}

// STEP 2
unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

// should this return something or just write?
unsigned char cpu_ram_write(struct cpu *cpu, int address, unsigned char value) {
  return cpu->ram[address] = value;
}