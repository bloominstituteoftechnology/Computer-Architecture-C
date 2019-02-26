#include "cpu.h"
#define ADDR_EMPTY_STACK 0XF4
#define DATA_LEN 6
#include <string.h>
#include <stdio.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_ram_write(struct cpu *cpu, unsigned char value, unsignded char address)
{
  cpu->ram[address] = value;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_load(struct cpu *cpu)
{
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

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  // initialize operands
  unsigned char operandA = NULL;
  unsigned char operandB = NULL;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int operands = instruction >> 6; 
    // 3. Get the appropriate value(s) of the operands following this instruction
    if(operands == 2)
    {
      operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
      operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);
    }
    else if (operands == 1)
    {
      operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    }
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = -1;
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
  cpu->SP = ADDR_EMPTY_STACK;
  cpu->E = 0;
}
