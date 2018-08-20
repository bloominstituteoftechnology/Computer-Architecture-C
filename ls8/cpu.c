#include "cpu.h"
#include <stdio.h>
#include <string.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  const int DATA_LEN = 6;
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8  /* instruction */
    0b00000000, /* argument 1 */
    0b00001000, /* argument 2 */
    0b01000111, // PRN R0 /* instruction */
    0b00000000, /* argument 1 */
    0b00000001  // HLT /* instruction */
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

unsigned char cpu_ram_read(struct cpu *cpu, int index){
  return cpu->ram[index];
}



/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;
  unsigned char valB = reg[regB];
  switch (op) {
    // case ALU_ADD:
    //   reg[regA = regA + regB;
    //   // TODO
    //   break;
    // case ALU_AND:
    //   regA = regA & regB;
    //   break;
    // case ALU_DEC:
    //   regA = regA - 1;
    //   break;
    // case ALU_SUB:
    //   regA = regA - regB;
    //   break;
    case ALU_MUL:
      reg[regA] *= valB;
      break;
    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  unsigned char *reg = cpu->reg;
  unsigned char PC = (int)cpu->PC;
  int running = 1; // True until we get a HLT instruction

  while (running) {
    unsigned char IR = cpu_ram_read(cpu, PC);
    unsigned char operandA = cpu_ram_read(cpu, PC+1);
    unsigned char operandB = cpu_ram_read(cpu, PC+2);

    switch(IR)
    {
      case LDI:
        reg[operandA] = operandB;
        PC+=3;
        break;
      case PRN:
        printf("%d\n", reg[operandA]);
        PC+=2;
        break;
      case HLT:
        running = 0;
        break;
      default:
          break;
    }
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;

  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}
