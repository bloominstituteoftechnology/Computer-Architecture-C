#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->RAM[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->RAM[address] = value;
}

void cpu_load(struct cpu *cpu)
{
  char data[6] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < 6; i++) {
    cpu->RAM[address++] = data[i];
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
      cpu->reg[regA] = regA * regB;
      printf("MUL instruciton for ALU provided\n"); 
      break;
    case ALU_ADD:
      cpu->reg[regA] = regA + regB;
      printf("ADD instruciton for ALU provided\n"); 
      break;
    default:
      printf("No instruciton for ALU provided\n"); 
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
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    // instruction to follow
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    // where to store the following value
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    // what to store
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);
    switch(IR){
      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->pc += 3;
        break;
      default:
        printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2);
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->pc += 2;
        break;
      case HLT:
        running = 0;
        break;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{

  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM

  cpu->pc = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->RAM, 0, sizeof(cpu->RAM));

}
