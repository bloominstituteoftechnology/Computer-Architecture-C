#include "cpu.h"
#include <string.h>
#include <stdio.h>

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
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
  unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
    return cpu->ram[address]; /* reads the cpu struct's contents at this specific location in memory */
  }

  unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char new_data) {
    cpu->ram[address] = new_data; /* set new data in memory */
  }

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

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC); /* IR = Instruction Register */
    
    // 2. Figure out how many operands this next instruction requires
    unsigned char op0 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char op1 = cpu_ram_read(cpu, cpu->PC + 2);
    // 3. Get the appropriate value(s) of the operands following this instruction
    
    // 4. switch() over it to decide on a course of action.
    /* case pseudocode for instruction handlers:
    LDI -- set cpu->registers first value to next value
    HLT -- set running to 0 and break
    PRN -- print current register value (same as first value in LDI)
    */
    // 5. Do whatever the instruction should do according to the spec.
    switch(IR) {

      case LDI:
      cpu->registers[op0] = op1;
      cpu->PC += 3;
      break;

      case PRN:
      printf("%d\n", cpu->registers[op0]);
      cpu->PC += 2;
      break;

      case HLT:
      running = 0;
      break;

      default:
      printf("Command 0x%02x\n not recognized.", IR);
      running = 0;
      break;
    }
    // 6. Move the PC to the next instruction.
    /* removed previous code and added this step to cases */
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  /* need to initialize PC, registers, and ram (all elements of CPU struct) */

  cpu->PC = 0;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}
