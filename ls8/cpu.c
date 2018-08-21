#include <stdlib.h> // Exit is stored here.
#include <stdio.h> // Because you're using printf() without a prototype.
#include "cpu.h"

// Helper functions for efficiency -- to prevent repeating yourself.
// For better readability and detect bugs.
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

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
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    case ALU_ADD:
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

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // Instruction or opcode.
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);

    // Its arguments or operands.
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);

    // printf("TRACE: %02x: %02x\n", cpu->pc, IR);

    // 2. switch() over it to decide on a course of action.
    switch(IR)
    {
      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->pc += 3;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->pc += 2; 
        break;
        
      case HLT:
        running = 0;
        break;

      default:
        printf("Unknown instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2);
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
  // Loads the bytes in address 0.
  cpu->pc = 0;

  // TODO: Zero registers and RAM
}
