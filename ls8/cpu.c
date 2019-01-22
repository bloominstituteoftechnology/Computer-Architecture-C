#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6

// Write the given value to the LS8's Ram at the given address
void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

// Read the value at address and return it
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
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
  unsigned char IR, operandA, operandB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    int add_to_pc = (IR >> 6) + 1;

    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    operandA = cpu_ram_read(cpu, cpu->PC+1 & 0xff); // 0xff is for "masking" to avoid implicit sign extension
    operandB = cpu_ram_read(cpu, cpu->PC+2 & 0xff);

    // TRACER
    printf("TRACE: %02X: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB);

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch(IR)
    {
      case LDI:
        cpu->reg[operandA] = operandB; // operandA here is the register, operandB is the value (see LS8-spec)
        break;
    }

    // 6. Move the PC to the next instruction.
    cpu->PC += add_to_pc;
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
}
